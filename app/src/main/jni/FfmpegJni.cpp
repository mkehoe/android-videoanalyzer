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




jobject buildMediaInfoJNI(JNIEnv * env, media_info* info) {

    jclass jmediaInfoClass = env->FindClass("com/mkehoe/videoanalyzer/data/MediaInfo");

    if(jmediaInfoClass != NULL) {
        jmethodID jInitMethodId = env->GetMethodID(jmediaInfoClass, "<init>", "()V");

        if (jInitMethodId != NULL) {
            jobject jMediaInfo = env->NewObject(jmediaInfoClass, jInitMethodId);

            if (jMediaInfo != NULL) {
                jmethodID setMetadata = env->GetMethodID(jmediaInfoClass, "setMetadata",
                                                            "(Ljava/lang/String;)V");
                if (setMetadata != NULL) {
                    jstring metadata = env->NewStringUTF(info->metadata);
                    if (metadata != NULL) {
                        env->CallVoidMethod(jMediaInfo, setMetadata, metadata);
                        env->DeleteLocalRef(metadata);
                    }
                }

                jmethodID setVideoCodec = env->GetMethodID(jmediaInfoClass, "setVideoCodec",
                                                         "(Ljava/lang/String;)V");
                if (setVideoCodec != NULL) {
                    jstring vcodec = env->NewStringUTF(info->codec);
                    if (vcodec != NULL) {
                        env->CallVoidMethod(jMediaInfo, setVideoCodec, vcodec);
                        env->DeleteLocalRef(vcodec);
                    }
                }

                jmethodID setPixelFormat = env->GetMethodID(jmediaInfoClass, "setPixelFormat",
                                                           "(Ljava/lang/String;)V");
                if (setPixelFormat != NULL) {
                    jstring pix_fmt = env->NewStringUTF(info->pix_fmt);
                    if (pix_fmt != NULL) {
                        env->CallVoidMethod(jMediaInfo, setPixelFormat, pix_fmt);
                        env->DeleteLocalRef(pix_fmt);
                    }
                }

                jmethodID setContainerFormat = env->GetMethodID(jmediaInfoClass, "setContainerFormat",
                                                            "(Ljava/lang/String;)V");
                if (setContainerFormat != NULL) {
                    jstring container = env->NewStringUTF(info->container);
                    if (container != NULL) {
                        env->CallVoidMethod(jMediaInfo, setContainerFormat, container);
                        env->DeleteLocalRef(container);
                    }
                }

                jmethodID setRecommendedThumbHeight = env->GetMethodID(jmediaInfoClass, "setRecommendedThumbHeight", "(I)V");
                if (setRecommendedThumbHeight != NULL) {
                    env->CallVoidMethod(jMediaInfo, setRecommendedThumbHeight, info->recommendedThumbHeight);
                }
                jmethodID setRecommendedThumbWidth = env->GetMethodID(jmediaInfoClass, "setRecommendedThumbWidth", "(I)V");
                if (setRecommendedThumbWidth != NULL) {
                    env->CallVoidMethod(jMediaInfo, setRecommendedThumbWidth, info->recommendedThumbWidth);
                }
                jmethodID setFramerateNum = env->GetMethodID(jmediaInfoClass, "setFramerateNum", "(I)V");
                if (setFramerateNum != NULL) {
                    env->CallVoidMethod(jMediaInfo, setFramerateNum, info->fr_num);
                }
                jmethodID setFramerateDen = env->GetMethodID(jmediaInfoClass, "setFramerateDen", "(I)V");
                if (setFramerateDen != NULL) {
                    env->CallVoidMethod(jMediaInfo, setFramerateDen, info->fr_den);
                }
                jmethodID setVideoBitrate = env->GetMethodID(jmediaInfoClass, "setVideoBitrate", "(J)V");
                if (setVideoBitrate != NULL) {
                    env->CallVoidMethod(jMediaInfo, setVideoBitrate, info->bitrate);
                }
                jmethodID setStreamCount = env->GetMethodID(jmediaInfoClass, "setStreamCount", "(I)V");
                if (setStreamCount != NULL) {
                    env->CallVoidMethod(jMediaInfo, setStreamCount, info->streamCount);
                }
                jmethodID setProgramCount = env->GetMethodID(jmediaInfoClass, "setProgramCount", "(I)V");
                if (setProgramCount != NULL) {
                    env->CallVoidMethod(jMediaInfo, setProgramCount, info->programCount);
                }
                jmethodID setProfile = env->GetMethodID(jmediaInfoClass, "setProfile", "(I)V");
                if (setProfile != NULL) {
                    env->CallVoidMethod(jMediaInfo, setProfile, info->profile);
                }
                jmethodID setLevel = env->GetMethodID(jmediaInfoClass, "setLevel", "(I)V");
                if (setLevel != NULL) {
                    env->CallVoidMethod(jMediaInfo, setLevel, info->level);
                }
                jmethodID setStreamDuration = env->GetMethodID(jmediaInfoClass, "setStreamDuration", "(J)V");
                if (setStreamDuration != NULL) {
                    env->CallVoidMethod(jMediaInfo, setStreamDuration, info->streamDuration);
                }
                jmethodID setIsSupported = env->GetMethodID(jmediaInfoClass, "setIsSupported", "(Z)V");
                if (setIsSupported != NULL) {
                    env->CallVoidMethod(jMediaInfo, setIsSupported, info->isSupported);
                }

            }
            return jMediaInfo;
        }

    }
    LOGE("Returning NULL from buildMediaInfoJNI");
    return NULL;
}
jobject buildFrameInfoJNI(JNIEnv * env, frame_info* info) {
    if(m_FrameInfoClass != NULL){
        jmethodID jInitMethodId = env->GetMethodID(m_FrameInfoClass, "<init>", "()V");

        if(jInitMethodId !=NULL) {
            jobject jFrameInfo = env->NewObject(m_FrameInfoClass, jInitMethodId);

            if (jFrameInfo != NULL) {
                jmethodID setIndex = env->GetMethodID(m_FrameInfoClass, "setFrameIndex", "(I)V");
                if (setIndex != NULL) {
                    env->CallVoidMethod(jFrameInfo, setIndex, info->frameIndex);
                }
                jmethodID setPictNum = env->GetMethodID(m_FrameInfoClass, "setPictureDisplayNumber",
                                                        "(I)V");
                if (setPictNum != NULL) {
                    env->CallVoidMethod(jFrameInfo, setPictNum, info->pictureDisplayNumber);
                }
                jmethodID setWidth = env->GetMethodID(m_FrameInfoClass, "setWidth", "(I)V");
                if (setWidth != NULL) {
                    env->CallVoidMethod(jFrameInfo, setWidth, info->width);
                }
                jmethodID setHeight = env->GetMethodID(m_FrameInfoClass, "setHeight", "(I)V");
                if (setHeight != NULL) {
                    env->CallVoidMethod(jFrameInfo, setHeight, info->height);
                }
                jmethodID setThumbWidth = env->GetMethodID(m_FrameInfoClass, "setThumbWidth",
                                                           "(I)V");
                if (setThumbWidth != NULL) {
                    env->CallVoidMethod(jFrameInfo, setThumbWidth, info->thumbWidth);
                }
                jmethodID setThumbHeight = env->GetMethodID(m_FrameInfoClass, "setThumbHeight",
                                                            "(I)V");
                if (setThumbHeight != NULL) {
                    env->CallVoidMethod(jFrameInfo, setThumbHeight, info->thumbHeight);
                }
                jmethodID setPictureType = env->GetMethodID(m_FrameInfoClass, "setPictureType",
                                                            "(Ljava/lang/String;)V");
                if (setPictureType != NULL) {
                    jstring pic_type = env->NewStringUTF(info->pictureType);
                    if (pic_type != NULL) {
                        env->CallVoidMethod(jFrameInfo, setPictureType, pic_type);
                        env->DeleteLocalRef(pic_type);
                    }
                }
                jmethodID setPacketPosition = env->GetMethodID(m_FrameInfoClass, "setPacketPosition",
                                                               "(J)V");
                if (setPacketPosition != NULL) {
                    env->CallVoidMethod(jFrameInfo, setPacketPosition, info->packetPosition);
                }

                jmethodID setPts = env->GetMethodID(m_FrameInfoClass, "setPts", "(J)V");
                if (setPts != NULL) {
                    env->CallVoidMethod(jFrameInfo, setPts, info->pts);
                }
                jmethodID setDts = env->GetMethodID(m_FrameInfoClass, "setDts", "(J)V");
                if (setDts != NULL) {
                    env->CallVoidMethod(jFrameInfo, setDts, info->dts);
                }
                jmethodID setIsInterlaced = env->GetMethodID(m_FrameInfoClass, "setIsInterlaced",
                                                             "(Z)V");
                if (setIsInterlaced != NULL) {
                    env->CallVoidMethod(jFrameInfo, setIsInterlaced, info->isInterlaced);
                }
                jmethodID setIsKeyFrame = env->GetMethodID(m_FrameInfoClass, "setIsKeyFrame",
                                                           "(Z)V");
                if (setIsKeyFrame != NULL) {
                    env->CallVoidMethod(jFrameInfo, setIsKeyFrame, info->isKeyFrame);
                }
                jmethodID setTimestamp = env->GetMethodID(m_FrameInfoClass, "setTimestamp", "(J)V");
                if (setTimestamp != NULL) {
                    env->CallVoidMethod(jFrameInfo, setTimestamp, info->timestamp);
                }
                jmethodID setDuration = env->GetMethodID(m_FrameInfoClass, "setDuration", "(J)V");
                if (setDuration != NULL) {
                    env->CallVoidMethod(jFrameInfo, setDuration, info->duration);
                }
                jmethodID setQuality = env->GetMethodID(m_FrameInfoClass, "setQuality", "(I)V");
                if (setQuality != NULL) {
                    env->CallVoidMethod(jFrameInfo, setQuality, info->quality);
                }

                return jFrameInfo;
            }
        }


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

extern "C"{

static void frameProcessedCallback(frame_info *frameInfo) {
    JNIEnv *env;
    LOGV("frameProcessedCallback");

    if (m_Jvm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGV("Attaching to JVM thread");
        if (m_Jvm->AttachCurrentThread(&env, NULL) != 0) {
            LOGE("Failed to attach to thread");
            return;
        }
    }


    jclass clazz = env->GetObjectClass(m_CallbackActivity);
    if (clazz != NULL) {
        jmethodID jThumbnailProcessedCallbackId = env->GetStaticMethodID(clazz, "thumbnailProcessedCallback",
                                              "(Landroid/graphics/Bitmap;Lcom/mkehoe/videoanalyzer/data/FrameInfo;)V");
        if(jThumbnailProcessedCallbackId == NULL){
            LOGE("Failed to find jThumbnailProcessedCallbackId");
        }

        jfieldID bitmapArrayFieldId = env->GetFieldID(clazz, "m_BitmapArr", "[Landroid/graphics/Bitmap;");
        if(bitmapArrayFieldId == NULL){
            LOGE("Failed to find bitmapArrayFieldId");
        }
        if (bitmapArrayFieldId != NULL && jThumbnailProcessedCallbackId != NULL) {
            jobjectArray bitmapArr = reinterpret_cast<jobjectArray>(env->GetObjectField(m_CallbackActivity, bitmapArrayFieldId));

            if (bitmapArr != NULL) {

                jobject jBitmap = env->GetObjectArrayElement(bitmapArr, frameInfo->frameIndex);

                copyDataToBitmap(env, jBitmap, frameInfo->imageBuffer, frameInfo->thumbHeight*frameInfo->thumbWidth*4);

                jobject jFrameInfo = buildFrameInfoJNI(env, frameInfo);

                env->CallStaticVoidMethod(clazz, jThumbnailProcessedCallbackId, jBitmap, jFrameInfo);
                env->DeleteLocalRef(jFrameInfo);
                env->DeleteLocalRef(jBitmap);
            }
            env->DeleteLocalRef(bitmapArr);
        }
    }

    if (frameInfo->frameIndex >= (m_iframeCount - 1)) {
        env->DeleteGlobalRef(m_CallbackActivity);
        env->DeleteGlobalRef(m_FrameInfoClass);

        LOGV("Detaching the JVM thread");
        m_Jvm->DetachCurrentThread();
    }
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

    m_VideoDecoder = new VideoDecoder();
    media_info* pMediaInfo = m_VideoDecoder->OpenFile(utf8);

    return buildMediaInfoJNI(env, pMediaInfo);
}

JNIEXPORT jint Java_com_mkehoe_videoanalyzer_MainActivity_GetThumbnailFrames(JNIEnv *env, jobject clazz,
                                                                jobjectArray buffers,
                                                                jint width, jint height, jint frameCount) {
    LOGV("GetThumbnailFrames");

    env->GetJavaVM( &m_Jvm);
    m_CallbackActivity = env->NewGlobalRef(clazz);
    m_iframeCount = frameCount;
    jclass jFrameInfoClass = env->FindClass("com/mkehoe/videoanalyzer/data/FrameInfo");
    m_FrameInfoClass = (jclass)env->NewGlobalRef(jFrameInfoClass);

    if (m_VideoDecoder != NULL) {
        void ** pVoidBuffers = new void *[frameCount];
        for (int i = 0; i < frameCount; i++) {

            jobject buffer = env->GetObjectArrayElement( buffers, i);
            pVoidBuffers[i] = env->GetDirectBufferAddress(buffer);
            env->DeleteLocalRef(buffer);
        }
        m_VideoDecoder->GetFrames(pVoidBuffers, width, height, frameCount, (FrameProcessingCallback) &frameProcessedCallback);
    }

}

JNIEXPORT jboolean Java_com_mkehoe_videoanalyzer_MainActivity_CloseFile(JNIEnv *env, jobject clazz) {
    LOGV("Close File");
    m_VideoDecoder->CloseFile();
    delete m_VideoDecoder;
    m_VideoDecoder = NULL;

}

}