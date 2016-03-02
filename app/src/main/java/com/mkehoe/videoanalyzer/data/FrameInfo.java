package com.mkehoe.videoanalyzer.data;

/**
 * Created by mkehoe on 2/29/2016.
 */
public class FrameInfo {
    int frameIndex;
    int width;
    int height;
    int thumbWidth;
    int thumbHeight;
    String pictureType;
    int pictureDisplayNumber;
    long packetPosition;
    long pts;
    long dts;
    boolean isInterlaced;
    boolean isKeyFrame;
    long timestamp;
    long duration;
    int quality;

    public FrameInfo(){

    }

    public int getFrameIndex() {
        return frameIndex;
    }

    public void setFrameIndex(int frameIndex) {
        this.frameIndex = frameIndex;
    }

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getThumbWidth() {
        return thumbWidth;
    }

    public void setThumbWidth(int thumbWidth) {
        this.thumbWidth = thumbWidth;
    }

    public int getThumbHeight() {
        return thumbHeight;
    }

    public void setThumbHeight(int thumbHeight) {
        this.thumbHeight = thumbHeight;
    }

    public String getPictureType() {
        return pictureType;
    }

    public void setPictureType(String pictureType) {
        this.pictureType = pictureType;
    }

    public int getPictureDisplayNumber() {
        return pictureDisplayNumber;
    }

    public void setPictureDisplayNumber(int pictureDisplayNumber) {
        this.pictureDisplayNumber = pictureDisplayNumber;
    }

    public long getPacketPosition() {
        return packetPosition;
    }

    public void setPacketPosition(long packetPosition) {
        this.packetPosition = packetPosition;
    }

    public long getPts() {
        return pts;
    }

    public void setPts(long pts) {
        this.pts = pts;
    }

    public long getDts() {
        return dts;
    }

    public void setDts(long dts) {
        this.dts = dts;
    }

    public boolean isInterlaced() {
        return isInterlaced;
    }

    public void setIsInterlaced(boolean isInterlaced) {
        this.isInterlaced = isInterlaced;
    }

    public boolean isKeyFrame() {
        return isKeyFrame;
    }

    public void setIsKeyFrame(boolean isKeyFrame) {
        this.isKeyFrame = isKeyFrame;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public long getDuration() {
        return duration;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }

    public int getQuality() {
        return quality;
    }

    public void setQuality(int quality) {
        this.quality = quality;
    }

}
