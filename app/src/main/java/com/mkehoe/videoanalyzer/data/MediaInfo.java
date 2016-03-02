package com.mkehoe.videoanalyzer.data;

/**
 * Created by mkehoe on 3/1/2016.
 */
public class MediaInfo {
    String metadata;
    String videoCodec;
    String pixelFormat;
    String containerFormat;
    int recommendedThumbHeight;
    int recommendedThumbWidth;
    int framerateNum;
    int framerateDen;
    long videoBitrate;
    int streamCount;
    int programCount;
    int profile;
    int level;
    long streamDuration;
    boolean isSupported;

    public MediaInfo(){

    }

    public String getMetadata() {
        return metadata;
    }

    public void setMetadata(String metadata) {
        this.metadata = metadata;
    }

    public String getVideoCodec() {
        return videoCodec;
    }

    public void setVideoCodec(String videoCodec) {
        this.videoCodec = videoCodec;
    }

    public String getPixelFormat() {
        return pixelFormat;
    }

    public void setPixelFormat(String pixelFormat) {
        this.pixelFormat = pixelFormat;
    }

    public String getContainerFormat() {
        return containerFormat;
    }

    public void setContainerFormat(String containerFormat) {
        this.containerFormat = containerFormat;
    }

    public int getRecommendedThumbHeight() {
        return recommendedThumbHeight;
    }

    public void setRecommendedThumbHeight(int recommendedThumbHeight) {
        this.recommendedThumbHeight = recommendedThumbHeight;
    }

    public int getRecommendedThumbWidth() {
        return recommendedThumbWidth;
    }

    public void setRecommendedThumbWidth(int recommendedThumbWidth) {
        this.recommendedThumbWidth = recommendedThumbWidth;
    }

    public int getFramerateNum() {
        return framerateNum;
    }

    public void setFramerateNum(int framerateNum) {
        this.framerateNum = framerateNum;
    }

    public int getFramerateDen() {
        return framerateDen;
    }

    public void setFramerateDen(int framerateDen) {
        this.framerateDen = framerateDen;
    }

    public long getVideoBitrate() {
        return videoBitrate;
    }

    public void setVideoBitrate(long videoBitrate) {
        this.videoBitrate = videoBitrate;
    }

    public int getStreamCount() {
        return streamCount;
    }

    public void setStreamCount(int streamCount) {
        this.streamCount = streamCount;
    }

    public int getProgramCount() {
        return programCount;
    }

    public void setProgramCount(int programCount) {
        this.programCount = programCount;
    }

    public int getProfile() {
        return profile;
    }

    public void setProfile(int profile) {
        this.profile = profile;
    }

    public int getLevel() {
        return level;
    }

    public void setLevel(int level) {
        this.level = level;
    }

    public long getStreamDuration() {
        return streamDuration;
    }

    public void setStreamDuration(long streamDuration) {
        this.streamDuration = streamDuration;
    }

    public boolean isSupported() {
        return isSupported;
    }

    public void setIsSupported(boolean isSupported) {
        this.isSupported = isSupported;
    }
}
