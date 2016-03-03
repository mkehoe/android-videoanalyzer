//
// Created by mkehoe on 2/27/2016.
//

#include "VideoDecoder.h"

#include <assert.h>
#include <android/log.h>
#include <jni.h>
#include <cstdlib>
#include <string.h>


#define TAG "VideoDecoder"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)



VideoDecoder::VideoDecoder() : m_bIsOpen(false), m_pFormatCtx(NULL), m_pCodec(NULL), m_iVideoStreamIndex(-1), m_pCodecCtx(NULL), processingThread(NULL) {
    av_register_all();
}

VideoDecoder::~VideoDecoder() {
    if(m_bIsOpen){
        CloseFile();
    }

}

media_info*  VideoDecoder::OpenFile(const char * file_url) {
    AVCodecContext *pCodecCtxOrig = NULL;
    if(m_bIsOpen){
        CloseFile();
    }

    assert(NULL != file_url);

    LOGV("Got string");
    // Open video file
    if (avformat_open_input(&m_pFormatCtx, file_url, NULL, NULL) != 0) {
        LOGV("Failed to open file");
        return false; // Couldn't open file
    }
    LOGV("Open File");
    // Retrieve stream information
    if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0) {
        return false; // Couldn't find stream information
    }

    // Dump information about file onto standard error
    //av_dump_format(m_pFormatCtx, 0, file_url, 0);

    // Find the first video stream
    for (int i = 0; i < m_pFormatCtx->nb_streams; i++)
        if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_iVideoStreamIndex = i;
            break;
        }
    if (m_iVideoStreamIndex == -1) {
        return false; // Didn't find a video stream
    }


    // Get a pointer to the codec context for the video stream
    pCodecCtxOrig = m_pFormatCtx->streams[m_iVideoStreamIndex]->codec;

    // Find the decoder for the video stream
    m_pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if (m_pCodec == NULL) {
        LOGV("Unsupported codec!\n");
        return false; // Codec not found
    }
    // Copy context
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
    if (avcodec_copy_context(m_pCodecCtx, pCodecCtxOrig) != 0) {
        LOGV("Couldn't copy codec context");
        return false; // Error copying codec context
    }

    avcodec_close(pCodecCtxOrig);

    // Open codec
    if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0) {
        return false; // Could not open codec
    }


    media_info* mediaInfo = new media_info;
    (*mediaInfo).recommendedThumbWidth = THUMB_SIZE_WIDTH;
    (*mediaInfo).recommendedThumbHeight = (int)((float)(THUMB_SIZE_WIDTH*m_pCodecCtx->height)/(float)m_pCodecCtx->width);

    (*mediaInfo).streamDuration = m_pFormatCtx->duration;
    (*mediaInfo).streamCount = m_pFormatCtx->nb_streams;
    (*mediaInfo).programCount = m_pFormatCtx->nb_programs;


    AVDictionaryEntry *tag = NULL;
    if(m_pFormatCtx->metadata != NULL) {
        int iMaxMetadataStrSize = 2000;
        char sBuffer[iMaxMetadataStrSize];
        char *pCursor = sBuffer;

        while ((tag = av_dict_get(m_pFormatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            pCursor += snprintf(pCursor, sizeof(sBuffer) - (pCursor - sBuffer), " %s: %s\n",
                                tag->key, tag->value);
        }

        (*mediaInfo).metadata = pCursor;
    }
    else{
        const char *text= "None\0";
        int size = (strlen(text)+1) * sizeof(char);
        char *pNoMeta = new char[size]();
        strncpy(pNoMeta, text, size);
        (*mediaInfo).metadata = pNoMeta;

    }
    (*mediaInfo).bitrate = m_pCodecCtx->bit_rate;
    (*mediaInfo).level = m_pCodecCtx->level;
    (*mediaInfo).profile = m_pCodecCtx->profile;
    (*mediaInfo).isSupported = true;
    (*mediaInfo).container = m_pFormatCtx->iformat->long_name;

    AVRational fr = m_pCodecCtx->framerate;
    (*mediaInfo).fr_num = fr.num;
    (*mediaInfo).fr_den = fr.den;

    AVPixelFormat pix_fmt = m_pCodecCtx->pix_fmt;
    if(AV_PIX_FMT_YUV420P == pix_fmt){
        const char *text= "YUV420p\0";
        int size = (strlen(text)+1) * sizeof(char);
        char *sPixType = new char[size]();
        strncpy(sPixType, text, size);
        (*mediaInfo).pix_fmt = sPixType;
    }
    else{
        (*mediaInfo).isSupported = false;
        const char *text= "Unsupported pixel format\0";
        int size = (strlen(text)+1) * sizeof(char);
        char *sPixType = new char[size]();
        strncpy(sPixType, text, size);
        (*mediaInfo).pix_fmt = sPixType;
    }

    AVCodecID codec = m_pCodecCtx->codec_id;
    if(AV_CODEC_ID_H264 == codec){
        const char *text= "H.264\0";
        int size = (strlen(text)+1) * sizeof(char);
        char *sPixType = new char[size]();
        strncpy(sPixType, text, size);
        (*mediaInfo).codec = sPixType;
    }
    else{
        (*mediaInfo).isSupported = false;
        const char *text= "Unsupported Codec\0";
        int size = (strlen(text)+1) * sizeof(char);
        char *sPixType = new char[size]();
        strncpy(sPixType, text, size);
        (*mediaInfo).codec = sPixType;
    }

    m_bIsOpen = true;
    return mediaInfo;

}
char * VideoDecoder::getFrameType(AVPictureType pic_type){

    switch(pic_type){
        case AV_PICTURE_TYPE_I: {
            const char *text= "I Frame\0";
            int size = (strlen(text)+1) * sizeof(char);
            char *sPicType = new char[size]();
            strncpy(sPicType, text, size);
            return sPicType;
            break;
        }
        case AV_PICTURE_TYPE_P:{
            const char *text= "P Frame\0";
            int size = (strlen(text)+1) * sizeof(char);
            char *sPicType = new char[size]();
            strncpy(sPicType, text, size);
            return sPicType;
            break;
        }
        case AV_PICTURE_TYPE_B:{
            const char *text= "B Frame\0";
            int size = (strlen(text)+1) * sizeof(char);
            char *sPicType = new char[size]();
            strncpy(sPicType, text, size);
            return sPicType;
            break;
        }
        default:{
            const char *text= "Unknown Frame\0";
            int size = (strlen(text)+1) * sizeof(char);
            char *sPicType = new char[size]();
            strncpy(sPicType, text, size);
            return sPicType;
            break;
        }
    }
}

void* VideoDecoder::ProcessFrames( void * context){
    VideoDecoder *_this = static_cast<VideoDecoder *>(context);
    AVFrame *pFrame = NULL;
    AVFrame *pFrameRGB = NULL;

    AVPacket packet;
    int frameFinished;
    int numBytes;
    uint8_t *buffer = NULL;

    struct SwsContext *sws_ctx = NULL;

    // Allocate video frame
    pFrame = av_frame_alloc();

    // Allocate an AVFrame structure
    pFrameRGB = av_frame_alloc();
    if (pFrameRGB == NULL || pFrame == NULL) {
        //return -1;
    }

    // Determine required buffer size and allocate buffer
    //numBytes=avpicture_get_size(AV_PIX_FMT_RGBA, _this->m_pCodecCtx->width,
                                //_this->m_pCodecCtx->height);
    numBytes=avpicture_get_size(AV_PIX_FMT_RGBA, _this->m_ProcessFramesArgs.width,
                                _this->m_ProcessFramesArgs.height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGBA,
                   _this->m_ProcessFramesArgs.width, _this->m_ProcessFramesArgs.height);

    //av_image_fill_arrays( pFrame->data, pFrame->linesize, NULL, AV_PIX_FMT_RGBA, m_pCodecCtx->width, m_pCodecCtx->height, 1);

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(_this->m_pCodecCtx->width,
                             _this->m_pCodecCtx->height,
                             _this->m_pCodecCtx->pix_fmt,
                             _this->m_ProcessFramesArgs.width,
                             _this->m_ProcessFramesArgs.height,
                             AV_PIX_FMT_RGBA,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL
    );

    int readFrameCount = 0;
    while (av_read_frame(_this->m_pFormatCtx, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == _this->m_iVideoStreamIndex) {
            // Decode video frame
            avcodec_decode_video2(_this->m_pCodecCtx, pFrame, &frameFinished, &packet);

            // Did we get a video frame?
            if (frameFinished) {
                // Convert the image from its native format to RGB
                sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
                          pFrame->linesize, 0, _this->m_pCodecCtx->height,
                          pFrameRGB->data, pFrameRGB->linesize);

                //memcpy(_this->m_ProcessFramesArgs.imageBuffers[readFrameCount], (void *) pFrameRGB->data[0], _this->m_ProcessFramesArgs.width * _this->m_ProcessFramesArgs.height * 4 );

                frame_info* frameInfo = new frame_info;
                (*frameInfo).frameIndex = readFrameCount;
                (*frameInfo).thumbHeight = _this->m_ProcessFramesArgs.height;
                (*frameInfo).thumbWidth = _this->m_ProcessFramesArgs.width;
                (*frameInfo).height = pFrame->height;
                (*frameInfo).width = pFrame->width;
                (*frameInfo).packetPosition = pFrame->pkt_pos;
                (*frameInfo).pts = pFrame->pts;
                (*frameInfo).dts = pFrame->pkt_dts;
                (*frameInfo).timestamp = pFrame->best_effort_timestamp;
                (*frameInfo).duration = pFrame->pkt_duration;
                (*frameInfo).pictureDisplayNumber = pFrame->display_picture_number;
                (*frameInfo).isInterlaced = (bool)pFrame->interlaced_frame;
                (*frameInfo).isKeyFrame = (bool)pFrame->key_frame;
                (*frameInfo).quality = pFrame->quality;
                AVPictureType pic_type = pFrame->pict_type;
                (*frameInfo).pictureType = _this->getFrameType(pic_type);

                //frameInfo->imageBuffer = _this->m_ProcessFramesArgs.imageBuffers[readFrameCount];
                frameInfo->imageBuffer = (void*)pFrameRGB->data[0];
                (_this->m_ProcessFramesArgs.callback)(frameInfo);
                delete frameInfo;
                readFrameCount++;
                if( (_this->m_ProcessFramesArgs.frameCount > 0) && (readFrameCount >= _this->m_ProcessFramesArgs.frameCount)) {
                    break;
                }
            }
        }
        // Free the packet that was allocated by av_read_frame
        av_packet_unref(&packet);
    }

    // Free the RGB image
    av_frame_free(&pFrameRGB);

    // Free the YUV frame
    av_frame_free(&pFrame);
    //return i;

    ///notify the listener to cleanup
    (_this->m_ProcessFramesArgs.cleanupCallback)();


}

void VideoDecoder::GetFrames(int width, int height, int frameCount, FrameProcessingCallback callback, ProcessingCleanupCallback cleanupCallback) {
    m_ProcessFramesArgs.height =height;
    m_ProcessFramesArgs.width = width;
    m_ProcessFramesArgs.frameCount = frameCount;
    m_ProcessFramesArgs.callback = callback;
    m_ProcessFramesArgs.cleanupCallback = cleanupCallback;
    pthread_create(&processingThread, NULL, &VideoDecoder::ProcessFrames, this );

}


void VideoDecoder::CloseFile(){

    // Close the codecs
    avcodec_close(m_pCodecCtx);

    // Close the video file
    avformat_close_input(&m_pFormatCtx);

}