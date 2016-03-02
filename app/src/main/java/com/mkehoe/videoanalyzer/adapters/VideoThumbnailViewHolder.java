package com.mkehoe.videoanalyzer.adapters;

import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import com.mkehoe.videoanalyzer.R;
import com.mkehoe.videoanalyzer.data.VideoThumbnailItem;

/**
 * Created by mkehoe on 3/1/2016.
 */
public class VideoThumbnailViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
    static String CLASS_TAG = "com.mkehoe.videoanalyzer.VideoThumbnailViewHolder";

    private VideoThumbnailItem mVideoThumbnailItem;
    private View rootLayoutView;

    public VideoThumbnailViewHolder(View view) {
        super(view);
        view.setOnClickListener(this);
        rootLayoutView = view;
    }

    public void setItem(VideoThumbnailItem item) {
        Log.e(CLASS_TAG, "Setting Item " + Integer.toString(item.getId()));
        ImageView thumbnail = (ImageView) rootLayoutView.findViewById(R.id.listview_item_video_thumbnail_imageview);
        //TextView idText = (TextView) rootLayoutView.findViewById(R.id.listview_item_video_thumbnail_text_view_id);
        //idText.setText(Integer.toString(item.getId()));
        thumbnail.setImageBitmap(item.getBitmap());
        mVideoThumbnailItem = item;
    }
    public VideoThumbnailItem getItem(){
        return mVideoThumbnailItem;
    }

    @Override
    public void onClick(View view) {
        Log.d(CLASS_TAG, "onClick " + getAdapterPosition() + " " + mVideoThumbnailItem);
    }
}
