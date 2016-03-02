package com.mkehoe.videoanalyzer.fragments;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.mkehoe.videoanalyzer.R;
import com.mkehoe.videoanalyzer.data.MediaInfo;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link MediaInfoFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class MediaInfoFragment extends Fragment {
    static String CLASS_TAG = "com.mkehoe.videoanalyzer.fragments.VideoSeekerFragment";

    TextView mProgramCountText = null;
    TextView mStreamCountText;
    TextView mVideoCodecText;
    TextView mPixelFormatText;
    TextView mFramerateNumText;
    TextView mFramerateDenText;
    TextView mCodecProfileText;
    TextView mCodecLevelText;
    TextView mVideoBitrateText;
    TextView mStreamDurationText;
    TextView mContainerText;
    boolean mNeedsUpdate = false;


    // TODO: Rename and change types of parameters
    private MediaInfo mMediaInfo;



    public MediaInfoFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @return A new instance of fragment MediaInfoFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static MediaInfoFragment newInstance(MediaInfo mediaInfo) {
        MediaInfoFragment fragment = new MediaInfoFragment();
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_media_info, container, false);
        loadUI(view);
        if(mNeedsUpdate){
            mNeedsUpdate = false;
            updateMediaInfo(mMediaInfo);
        }
        return view;
    }

    private void loadUI(View view) {
        mProgramCountText = (TextView) view.findViewById(R.id.program_count);
        mStreamCountText = (TextView) view.findViewById(R.id.stream_count);
        mVideoCodecText = (TextView) view.findViewById(R.id.video_codec);
        mPixelFormatText = (TextView) view.findViewById(R.id.pixel_format);
        mFramerateNumText = (TextView) view.findViewById(R.id.framerate_numerator);
        mFramerateDenText = (TextView) view.findViewById(R.id.framerate_denominator);
        mCodecProfileText = (TextView) view.findViewById(R.id.video_profile);
        mCodecLevelText = (TextView) view.findViewById(R.id.video_level);
        mVideoBitrateText = (TextView) view.findViewById(R.id.video_bitrate);
        mStreamDurationText = (TextView) view.findViewById(R.id.stream_duration);
        mContainerText = (TextView) view.findViewById(R.id.container);
    }

    public void updateMediaInfo(MediaInfo mediaInfo) {
        mMediaInfo = mediaInfo;
        if(mProgramCountText == null) {
            Log.e(CLASS_TAG, "updateMediaInfo UI not ready yet");
            mNeedsUpdate = true;
            return;
        }
        mProgramCountText.setText(Integer.toString(mediaInfo.getProgramCount()));
        mStreamCountText.setText(Integer.toString(mediaInfo.getStreamCount()));
        mVideoCodecText.setText(mediaInfo.getVideoCodec());
        mPixelFormatText.setText(mediaInfo.getPixelFormat());
        mFramerateNumText.setText(Integer.toString(mediaInfo.getFramerateNum()));
        mFramerateDenText.setText(Integer.toString(mediaInfo.getFramerateDen()));
        mCodecProfileText.setText(Integer.toString(mediaInfo.getProfile()));
        mCodecLevelText.setText(Integer.toString(mediaInfo.getLevel()));
        mVideoBitrateText.setText(Long.toString(mediaInfo.getVideoBitrate()));
        mStreamDurationText.setText(Long.toString(mediaInfo.getStreamDuration()));
        mContainerText.setText(mediaInfo.getContainerFormat());
    }

}
