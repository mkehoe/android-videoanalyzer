package com.mkehoe.videoanalyzer.adapters;

import android.app.Activity;
import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.mkehoe.videoanalyzer.R;
import com.mkehoe.videoanalyzer.data.FrameInfo;
import com.mkehoe.videoanalyzer.data.VideoThumbnailItem;

import java.util.List;

/**
 * Created by mkehoe on 2/28/2016.
 */
public class VideoThumbnailAdapter extends RecyclerView.Adapter<VideoThumbnailAdapter.VideoThumbnailViewHolder>{
    static String CLASS_TAG = "com.mkehoe.videoanalyzer.VideoThumbnailAdapter";


    List<VideoThumbnailItem> mVideoThumbnails = null;
    int mSelectedItem = 0;
    public IVideoThumbnailAdpater mListener;

    public VideoThumbnailAdapter(List<VideoThumbnailItem> thumbnails,IVideoThumbnailAdpater listener) {
        mVideoThumbnails = thumbnails;
        mListener = listener;
    }

    @Override
    public VideoThumbnailAdapter.VideoThumbnailViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.listview_item_video_thumbnail, parent, false);

        return new VideoThumbnailViewHolder(view);
    }

    @Override
    public void onBindViewHolder(VideoThumbnailViewHolder holder, int position) {
        holder.setItem(mVideoThumbnails.get(position));
    }

    @Override
    public int getItemCount() {
        return mVideoThumbnails.size();
    }

    public void updateSelected(){

    }

    public interface IVideoThumbnailAdpater{
        void onUpdateFrameInfoNeeded(FrameInfo frameInfo);
    }

    public class VideoThumbnailViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        private VideoThumbnailItem mVideoThumbnailItem;
        private View rootLayoutView;

        public VideoThumbnailViewHolder(View view) {
            super(view);
            view.setClickable(true);
            view.setOnClickListener(this);
            rootLayoutView = view;

        }

        public void setItem(VideoThumbnailItem item) {
            ImageView thumbnail = (ImageView) rootLayoutView.findViewById(R.id.listview_item_video_thumbnail_imageview);
            thumbnail.setImageBitmap(item.getBitmap());
            mVideoThumbnailItem = item;
        }
        public VideoThumbnailItem getItem(){
            return mVideoThumbnailItem;
        }

        @Override
        public void onClick(View view) {
            Log.d(CLASS_TAG, "onClick " + getAdapterPosition() + " " + mVideoThumbnailItem);
            if(mListener != null){
                mListener.onUpdateFrameInfoNeeded(mVideoThumbnailItem.getFrameInfo());
            }
        }

    }
}
