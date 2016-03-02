package com.mkehoe.videoanalyzer.fragments;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.mkehoe.videoanalyzer.R;
import com.mkehoe.videoanalyzer.adapters.VideoThumbnailAdapter;
import com.mkehoe.videoanalyzer.data.FrameInfo;
import com.mkehoe.videoanalyzer.data.VideoThumbnailItem;

import org.w3c.dom.Text;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;

/**
 * A fragment with a Google +1 button.
 * Activities that contain this fragment must implement the
 * to handle interaction events.
 * create an instance of this fragment.
 */
public class VideoSeekerFragment extends Fragment {
    static String CLASS_TAG = "com.mkehoe.videoanalyzer.fragments.VideoSeekerFragment";

    private RecyclerView mVideoThumbnailListView = null;
    private ArrayList<VideoThumbnailItem> mThumbnailList = new ArrayList<>();
    private VideoThumbnailAdapter mThumbnailAdapter = null;

    private TextView mFrameIndexText;
    private TextView mDisplayPictureNumberText;
    private TextView mWidthText;
    private TextView mHeightText;
    private TextView mPictureTypeText;
    private TextView mPacketPositionText;
    private TextView mPTSText;
    private TextView mDTSText;
    private TextView mInterlacedText;
    private TextView mKeyframeText;
    private TextView mTimestampText;
    private TextView mDurationText;
    private TextView mQualityText;

    public VideoSeekerFragment() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        final View view = inflater.inflate(R.layout.fragment_video_seeker, container, false);

        mVideoThumbnailListView = (RecyclerView)view.findViewById(R.id.video_seek_bar);
        mThumbnailAdapter = new VideoThumbnailAdapter(mThumbnailList, new VideoThumbnailAdapter.IVideoThumbnailAdpater() {
            @Override
            public void onUpdateFrameInfoNeeded(FrameInfo frameInfo) {
                updateFrameData(frameInfo);
            }
        });

        LinearLayoutManager llm = new LinearLayoutManager(getContext());
        llm.setOrientation(LinearLayoutManager.HORIZONTAL);
        mVideoThumbnailListView.setLayoutManager(llm);
        mVideoThumbnailListView.setAdapter(mThumbnailAdapter);


        loadUI(view);

        return view;
    }
    private void loadUI(View view){
        mFrameIndexText = (TextView)view.findViewById(R.id.frame_index);
        mDisplayPictureNumberText = (TextView)view.findViewById(R.id.display_picture_number);
        mWidthText = (TextView)view.findViewById(R.id.video_width );
        mHeightText = (TextView)view.findViewById(R.id.video_height);
        mPictureTypeText = (TextView)view.findViewById(R.id.frame_type);
        mPacketPositionText = (TextView)view.findViewById(R.id.packet_position);
        mPTSText = (TextView)view.findViewById(R.id.pts);
        mDTSText = (TextView)view.findViewById(R.id.dts);
        mInterlacedText = (TextView)view.findViewById(R.id.interlaced);
        mKeyframeText = (TextView)view.findViewById(R.id.keyframe);
        mTimestampText = (TextView)view.findViewById(R.id.timestamp);
        mDurationText = (TextView)view.findViewById(R.id.duration);
        mQualityText = (TextView)view.findViewById(R.id.quality);
    }

    public void updateFrameData(FrameInfo frameInfo){
        mFrameIndexText.setText(Integer.toString(frameInfo.getFrameIndex()));
        mDisplayPictureNumberText.setText(Integer.toString(frameInfo.getPictureDisplayNumber()));

        mWidthText.setText(Integer.toString(frameInfo.getWidth()));
        mHeightText.setText(Integer.toString(frameInfo.getHeight()));
        mPictureTypeText.setText(frameInfo.getPictureType());
        mPacketPositionText.setText(Long.toString(frameInfo.getPacketPosition()));
        mPTSText.setText(Long.toString(frameInfo.getPts()));
        mDTSText.setText(Long.toString(frameInfo.getDts()));
        mInterlacedText.setText(Boolean.toString(frameInfo.isInterlaced()));
        mKeyframeText.setText(Boolean.toString(frameInfo.isKeyFrame()));
        mTimestampText.setText(Long.toString(frameInfo.getTimestamp()));
        mDurationText.setText(Long.toString(frameInfo.getDuration()));
        mQualityText.setText(Integer.toString(frameInfo.getQuality()));
    }

    public void addThumbnail(Bitmap bitmap, FrameInfo frameInfo){

        VideoThumbnailItem thumb = new VideoThumbnailItem();
        thumb.setFrameInfo(frameInfo);
        thumb.setBitmap(bitmap);
        thumb.setId(frameInfo.getFrameIndex());
        mThumbnailList.add(thumb);

        if(mThumbnailAdapter != null) {
            mThumbnailAdapter.notifyDataSetChanged();
        }

    }
    @Override
    public void onStart(){
        super.onStart();
    }

    @Override
    public void onActivityCreated (Bundle savedInstanceState){
        super.onActivityCreated(savedInstanceState);
    }

    @Override
    public void onResume() {
        super.onResume();

    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }

}
