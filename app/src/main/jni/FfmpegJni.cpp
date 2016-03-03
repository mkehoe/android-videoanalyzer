//
// Created by mkehoe on 2/26/2016.
//
#include "VideoDecoder.h"
#include <assert.h>
#include <jni.h>

#include <android/bitmap.h>
#include <android/log.h>


#define TAG "FfmpegJni_cpp"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

static VideoDecoder *m_VideoDecoder = NULL;
static JavaVM* m_Jvm = 0;
static jobject m_CallbackActivity = 0; // GlobalRef
static jint m_iframeCount = -1;
static jclass m_FrameInfoClass = NULL;
static jclass m_BitmapConfig = NULL;
static jfieldID m_RGBA8888FieldID = NULL;
static jclass m_BitmapClass = NULL;
static jmethodID m_CreateBitmapMethodID = NULL;
static jclass m_jmediaInfoClass;
static jmethodID m_jMediaInfoInitMethodId;
static jmethodID m_setMetadata;
static jmethodID m_setVideoCodec;
static jmethodID m_setPixelFormat;
static jmethodID m_setContainerFormat;
static jmethodID m_setRecommendedThumbHeight;
static jmethodID m_setRecommendedThumbWidth;
static jmethodID m_setFramerateNum;
static jmethodID m_setFramerateDen;
static jmethodID m_setVideoBitrate;
static jmethodID m_setStreamCount;
static jmethodID m_setProgramCount;
static jmethodID m_setProfile;
static jmethodID m_setLevel;
static jmethodID m_setStreamDuration;
static jmethodID m_setIsSupported;
static jmethodID m_jFrameInfoInitMethodId;
static jmethodID m_setIndex;
static jmethodID m_setPictNum;
static jmethodID m_setWidth;
static jmethodID m_setHeight;
static jmethodID m_setThumbWidth;
static jmethodID m_setThumbHeight;
static jmethodID m_setPictureType;
static jmethodID m_setIsInterlaced;
static jmethodID m_setIsKeyFrame;
static jmethodID m_setTimestamp;
static jmethodID m_setPacketPosition;
static jmethodID m_setPts;
static jmethodID m_setDts;
static jmethodID m_setDuration;
static jmethodID m_setQuality;
static jmethodID m_jThumbnailProcessedCallbackId;
static jclass m_jMainActivityClass;



void setupMediaInfo(JNIEnv* env) {
    {
        jclass jmediaInfoClass = env->FindClass("com/mkehoe/videoanalyzer/data/MediaInfo");
        m_jmediaInfoClass = (jclass) env->NewGlobalRef(jmediaInfoClass);
        env->DeleteLocalRef(jmediaInfoClass);
    }

    m_jMediaInfoInitMethodId = env->GetMethodID(m_jmediaInfoClass, "<init>", "()V");
    m_setMetadata = env->GetMethodID(m_jmediaInfoClass, "setMetadata",
                                             "(Ljava/lang/String;)V");
    m_setVideoCodec = env->GetMethodID(m_jmediaInfoClass, "setVideoCodec",
                                               "(Ljava/lang/String;)V");
    m_setPixelFormat = env->GetMethodID(m_jmediaInfoClass, "setPixelFormat",
                                                "(Ljava/lang/String;)V");
    m_setContainerFormat = env->GetMethodID(m_jmediaInfoClass, "setContainerFormat",
                                                    "(Ljava/lang/String;)V");
    m_setRecommendedThumbHeight = env->GetMethodID(m_jmediaInfoClass,
                                                           "setRecommendedThumbHeight", "(I)V");
    m_setRecommendedThumbWidth = env->GetMethodID(m_jmediaInfoClass,
                                                          "setRecommendedThumbWidth", "(I)V");
    m_setFramerateNum = env->GetMethodID(m_jmediaInfoClass, "setFramerateNum", "(I)V");
    m_setFramerateDen = env->GetMethodID(m_jmediaInfoClass, "setFramerateDen", "(I)V");
    m_setVideoBitrate = env->GetMethodID(m_jmediaInfoClass, "setVideoBitrate", "(J)V");
    m_setStreamCount = env->GetMethodID(m_jmediaInfoClass, "setStreamCount", "(I)V");
    m_setProgramCount = env->GetMethodID(m_jmediaInfoClass, "setProgramCount", "(I)V");
    m_setProfile = env->GetMethodID(m_jmediaInfoClass, "setProfile", "(I)V");
    m_setLevel = env->GetMethodID(m_jmediaInfoClass, "setLevel", "(I)V");
    m_setStreamDuration = env->GetMethodID(m_jmediaInfoClass, "setStreamDuration", "(J)V");
    m_setIsSupported = env->GetMethodID(m_jmediaInfoClass, "setIsSupported", "(Z)V");
}

jobject buildMediaInfoJNI(JNIEnv * env, media_info* info) {
    jobject jMediaInfo = env->NewObject(m_jmediaInfoClass, m_jMediaInfoInitMethodId);
    if (jMediaInfo != NULL) {

        jstring metadata = env->NewStringUTF(info->metadata);
        if (metadata != NULL) {
            env->CallVoidMethod(jMediaInfo, m_setMetadata, metadata);
            env->DeleteLocalRef(metadata);
        }

        jstring vcodec = env->NewStringUTF(info->codec);
        if (vcodec != NULL) {
            env->CallVoidMethod(jMediaInfo, m_setVideoCodec, vcodec);
            env->DeleteLocalRef(vcodec);
        }

        jstring pix_fmt = env->NewStringUTF(info->pix_fmt);
        if (pix_fmt != NULL) {
            env->CallVoidMethod(jMediaInfo, m_setPixelFormat, pix_fmt);
            env->DeleteLocalRef(pix_fmt);
        }

        jstring container = env->NewStringUTF(info->container);
        if (container != NULL) {
            env->CallVoidMethod(jMediaInfo, m_setContainerFormat, container);
            env->DeleteLocalRef(container);
        }

        env->CallVoidMethod(jMediaInfo, m_setRecommendedThumbHeight, info->recommendedThumbHeight);
        env->CallVoidMethod(jMediaInfo, m_setRecommendedThumbWidth, info->recommendedThumbWidth);
        env->CallVoidMethod(jMediaInfo, m_setFramerateNum, info->fr_num);
        env->CallVoidMethod(jMediaInfo, m_setFramerateDen, info->fr_den);
        env->CallVoidMethod(jMediaInfo, m_setVideoBitrate, info->bitrate);
        env->CallVoidMethod(jMediaInfo, m_setStreamCount, info->streamCount);
        env->CallVoidMethod(jMediaInfo, m_setProgramCount, info->programCount);
        env->CallVoidMethod(jMediaInfo, m_setProfile, info->profile);
        env->CallVoidMethod(jMediaInfo, m_setLevel, info->level);
        env->CallVoidMethod(jMediaInfo, m_setStreamDuration, info->streamDuration);
        env->CallVoidMethod(jMediaInfo, m_setIsSupported, info->isSupported);
        return jMediaInfo;
    }
    LOGE("Returning NULL from buildMediaInfoJNI");
    return NULL;
}
void setupFrameInfo(JNIEnv * env){
    {
        jclass jFrameInfoClass = env->FindClass("com/mkehoe/videoanalyzer/data/FrameInfo");
        m_FrameInfoClass = (jclass) env->NewGlobalRef(jFrameInfoClass);
        env->DeleteLocalRef(jFrameInfoClass);
    }

    m_jFrameInfoInitMethodId = env->GetMethodID(m_FrameInfoClass, "<init>", "()V");
    m_setIndex = env->GetMethodID(m_FrameInfoClass, "setFrameIndex", "(I)V");
    m_setPictNum = env->GetMethodID(m_FrameInfoClass, "setPictureDisplayNumber", "(I)V");
    m_setWidth = env->GetMethodID(m_FrameInfoClass, "setWidth", "(I)V");
    m_setHeight = env->GetMethodID(m_FrameInfoClass, "setHeight", "(I)V");
    m_setThumbWidth = env->GetMethodID(m_FrameInfoClass, "setThumbWidth", "(I)V");
    m_setThumbHeight = env->GetMethodID(m_FrameInfoClass, "setThumbHeight", "(I)V");
    m_setPictureType = env->GetMethodID(m_FrameInfoClass, "setPictureType", "(Ljava/lang/String;)V");
    m_setIsInterlaced = env->GetMethodID(m_FrameInfoClass, "setIsInterlaced", "(Z)V");
    m_setIsKeyFrame = env->GetMethodID(m_FrameInfoClass, "setIsKeyFrame", "(Z)V");
    m_setTimestamp = env->GetMethodID(m_FrameInfoClass, "setTimestamp", "(J)V");
    m_setPacketPosition = env->GetMethodID(m_FrameInfoClass, "setPacketPosition", "(J)V");
    m_setPts = env->GetMethodID(m_FrameInfoClass, "setPts", "(J)V");
    m_setDts = env->GetMethodID(m_FrameInfoClass, "setDts", "(J)V");
    m_setDuration = env->GetMethodID(m_FrameInfoClass, "setDuration", "(J)V");
    m_setQuality = env->GetMethodID(m_FrameInfoClass, "setQuality", "(I)V");
}
jobject buildFrameInfoJNI(JNIEnv * env, frame_info* info) {
    jobject jFrameInfo = env->NewObject(m_FrameInfoClass, m_jFrameInfoInitMethodId);

    if (jFrameInfo != NULL) {
        env->CallVoidMethod(jFrameInfo, m_setIndex, info->frameIndex);
        env->CallVoidMethod(jFrameInfo, m_setPictNum, info->pictureDisplayNumber);
        env->CallVoidMethod(jFrameInfo, m_setWidth, info->width);
        env->CallVoidMethod(jFrameInfo, m_setHeight, info->height);
        env->CallVoidMethod(jFrameInfo, m_setThumbWidth, info->thumbWidth);
        env->CallVoidMethod(jFrameInfo, m_setThumbHeight, info->thumbHeight);
        jstring pic_type = env->NewStringUTF(info->pictureType);
        if (pic_type != NULL) {
            env->CallVoidMethod(jFrameInfo, m_setPictureType, pic_type);
            env->DeleteLocalRef(pic_type);
        }
        env->CallVoidMethod(jFrameInfo, m_setPacketPosition, info->packetPosition);
        env->CallVoidMethod(jFrameInfo, m_setPts, info->pts);
        env->CallVoidMethod(jFrameInfo, m_setDts, info->dts);
        env->CallVoidMethod(jFrameInfo, m_setIsInterlaced, info->isInterlaced);
        env->CallVoidMethod(jFrameInfo, m_setIsKeyFrame, info->isKeyFrame);
        env->CallVoidMethod(jFrameInfo, m_setTimestamp, info->timestamp);
        env->CallVoidMethod(jFrameInfo, m_setDuration, info->duration);
        env->CallVoidMethod(jFrameInfo, m_setQuality, info->quality);
        return jFrameInfo;
    }
    LOGE("Returning NULL from buildFrameInfoJNI");
    return NULL;
}

void copyDataToBitmap(JNIEnv* env, jobject bitmap, const void* data, int dataSize){
    /*AndroidBitmapInfo info;
                AndroidBitmap_getInfo(env, bitmap, &info);
                LOGV("Info.width=%d", info.width);
                LOGV("Info.height=%d", info.height);
                LOGV("Info.stride=%d", info.stride);
                LOGV("Info.format=%d", info.format);
                LOGV("datasize=%d",dataSize);
                //LOGV("thumbHeight=%d",frameInfo->thumbHeight);*/


    //memcpy the pixel data
    void * pixelAddr;
    int res = AndroidBitmap_lockPixels(env, bitmap, &pixelAddr);
    if(res >= 0 ) {
        memcpy(pixelAddr, data, (size_t)(dataSize));
        AndroidBitmap_unlockPixels(env, bitmap);
    }
}
void setupBitmap(JNIEnv * env){
    jclass jBitmapConfig = env->FindClass("android/graphics/Bitmap$Config");
    m_BitmapConfig = (jclass)env->NewGlobalRef(jBitmapConfig);
    jclass jBitmapClass = env->FindClass("android/graphics/Bitmap");
    m_BitmapClass = (jclass)env->NewGlobalRef(jBitmapClass);
    m_RGBA8888FieldID = env->GetStaticFieldID(m_BitmapConfig, "ARGB_8888",
                                              "Landroid/graphics/Bitmap$Config;");
    m_CreateBitmapMethodID = env->GetStaticMethodID(m_BitmapClass, "createBitmap",
                                                    "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
}

jobject createBitmap(JNIEnv * env, int width, int height){

    jobject jRGBA8888Obj = env->GetStaticObjectField(m_BitmapConfig, m_RGBA8888FieldID);
    if(jRGBA8888Obj != NULL) {
        jobject jBitmap = env->CallStaticObjectMethod(m_BitmapClass, m_CreateBitmapMethodID,
                                       width,
                                       height,
                                       jRGBA8888Obj);


        env->DeleteLocalRef(jRGBA8888Obj);
        return jBitmap;
    }


    LOGE("createBitmap Failed returning null");
    return NULL;

}

extern "C"{

static void frameProcessedCallback(frame_info *frameInfo) {
    JNIEnv *env;
    LOGV("frameProcessedCallback %d", frameInfo->frameIndex);

    if (m_Jvm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGV("Attaching to JVM thread");
        if (m_Jvm->AttachCurrentThread(&env, NULL) != 0) {
            LOGE("Failed to attach to thread");
            return;
        }
    }

    jobject jBitmap = createBitmap(env, frameInfo->thumbWidth, frameInfo->thumbHeight);
    copyDataToBitmap(env, jBitmap, frameInfo->imageBuffer,
                     frameInfo->thumbHeight * frameInfo->thumbWidth * 4);

    jobject jFrameInfo = buildFrameInfoJNI(env, frameInfo);

    env->CallStaticVoidMethod(m_jMainActivityClass, m_jThumbnailProcessedCallbackId, jBitmap, jFrameInfo);

    env->DeleteLocalRef(jFrameInfo);
    env->DeleteLocalRef(jBitmap);

}

static void processingCleanupCallback() {
    JNIEnv * env;
    LOGV("processingCleanupCallback");
    if (m_Jvm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGV("Attaching to JVM thread");
        if (m_Jvm->AttachCurrentThread(&env, NULL) != 0) {
            LOGE("Failed to attach to thread");
            return;
        }
    }

    env->DeleteGlobalRef(m_jMainActivityClass);
    env->DeleteGlobalRef(m_BitmapClass);
    env->DeleteGlobalRef(m_BitmapConfig);
    env->DeleteGlobalRef(m_CallbackActivity);
    env->DeleteGlobalRef(m_jmediaInfoClass);
    env->DeleteGlobalRef(m_FrameInfoClass);

    m_Jvm->DetachCurrentThread();

}

JNIEXPORT jobject Java_com_mkehoe_videoanalyzer_MainActivity_OpenFile(JNIEnv *env, jobject clazz,
                                                             jstring url) {

    LOGV("Opening File");
    const char *utf8 = env->GetStringUTFChars(url, NULL);
    assert(utf8 != NULL);


    if(m_VideoDecoder != NULL){
        m_VideoDecoder->CloseFile();
        delete m_VideoDecoder;
        m_VideoDecoder = NULL;
    }

    setupMediaInfo(env);

    m_VideoDecoder = new VideoDecoder();
    media_info* pMediaInfo = m_VideoDecoder->OpenFile(utf8);

    return buildMediaInfoJNI(env, pMediaInfo);
}

JNIEXPORT jint Java_com_mkehoe_videoanalyzer_MainActivity_GetThumbnailFrames(JNIEnv *env, jobject clazz,
                                                                jint width, jint height, jint frameCount) {
    LOGV("GetThumbnailFrames");


    env->GetJavaVM( &m_Jvm);
    m_CallbackActivity = env->NewGlobalRef(clazz);
    jclass jMainActivityClass = env->GetObjectClass(m_CallbackActivity);
    m_jMainActivityClass = (jclass)env->NewGlobalRef(jMainActivityClass);

    m_jThumbnailProcessedCallbackId = env->GetStaticMethodID(m_jMainActivityClass,
                                                                         "thumbnailProcessedCallback",
                                                                         "(Landroid/graphics/Bitmap;Lcom/mkehoe/videoanalyzer/data/FrameInfo;)V");
    env->DeleteLocalRef(jMainActivityClass);
    setupFrameInfo(env);
    setupBitmap(env);

    m_iframeCount = frameCount;

    if (m_VideoDecoder != NULL) {
        m_VideoDecoder->GetFrames(width, height, frameCount,
                                  (FrameProcessingCallback) &frameProcessedCallback,
                                  (ProcessingCleanupCallback) &processingCleanupCallback);
    }


}

JNIEXPORT jboolean Java_com_mkehoe_videoanalyzer_MainActivity_CloseFile(JNIEnv *env, jobject clazz) {
    LOGV("Close File");
    m_VideoDecoder->CloseFile();
    delete m_VideoDecoder;
    m_VideoDecoder = NULL;

}

}