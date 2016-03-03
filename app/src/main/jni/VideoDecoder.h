//
// Created by mkehoe on 2/27/2016.
//

#ifndef VIDEOANALYZER_VIDEODECODER_H
#define VIDEOANALYZER_VIDEODECODER_H

#include <jni.h>
#include <pthread.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include <libavutil/imgutils.h>
}

class VideoDecoder;

struct media_info {
    int64_t bitrate;
    int streamCount;
    int programCount;
    int64_t streamDuration;
    int profile;
    int level;
    int fr_num;
    int fr_den;
    const char * pix_fmt;
    const char * codec;
    const char * metadata;
    const char * container;
    bool isSupported;
    int recommendedThumbHeight;
    int recommendedThumbWidth;
};

struct frame_info {
    int frameIndex;
    int width;
    int height;
    int thumbWidth;
    int thumbHeight;
    const char * pictureType;
    int pictureDisplayNumber;
    int64_t packetPosition;
    int64_t pts;
    int64_t dts;
    bool isInterlaced;
    bool isKeyFrame;
    int64_t timestamp;
    int64_t duration;
    int quality;
    const void *imageBuffer;
};

typedef void (*FrameProcessingCallback)(frame_info*);
typedef void (*ProcessingCleanupCallback)();

struct processing_thread_args {
    int width;
    int height;
    int frameCount;
    FrameProcessingCallback callback;
    ProcessingCleanupCallback cleanupCallback;
};

const int THUMB_SIZE_WIDTH = 220;

class VideoDecoder {
public:
    VideoDecoder();
    ~VideoDecoder();
    media_info*  OpenFile(const char *file_url);
    void CloseFile();
    void GetFrames( int width, int height, int frameCount, FrameProcessingCallback callback, ProcessingCleanupCallback cleanupCallback);

private:
    static void* ProcessFrames( void * context);
    char * getFrameType(AVPictureType pic_type);

    bool m_bIsOpen;
    AVFormatContext *m_pFormatCtx;
    AVCodec *m_pCodec;
    AVCodecContext *m_pCodecCtx;
    int m_iVideoStreamIndex;
    pthread_t processingThread;
    struct processing_thread_args m_ProcessFramesArgs;


};


#endif //VIDEOANALYZER_VIDEODECODER_H
