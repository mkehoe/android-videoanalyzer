package com.mkehoe.videoanalyzer.data;

import android.graphics.Bitmap;

/**
 * Created by mkehoe on 2/28/2016.
 */
public class VideoThumbnailItem {
    private int id;
    private Bitmap bitmap;
    private FrameInfo frameInfo;

    public Bitmap getBitmap() {
        return bitmap;
    }

    public void setBitmap(Bitmap bitmap) {
        this.bitmap = bitmap;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public FrameInfo getFrameInfo() {
        return frameInfo;
    }

    public void setFrameInfo(FrameInfo frameInfo) {
        this.frameInfo = frameInfo;
    }

}
