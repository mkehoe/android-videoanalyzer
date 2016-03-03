package com.mkehoe.videoanalyzer;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.NavigationView;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import com.mkehoe.videoanalyzer.data.FrameInfo;
import com.mkehoe.videoanalyzer.data.MediaInfo;
import com.mkehoe.videoanalyzer.fragments.FileChooserFragment;
import com.mkehoe.videoanalyzer.fragments.MediaInfoFragment;
import com.mkehoe.videoanalyzer.fragments.VideoSeekerFragment;



public class MainActivity extends AppCompatActivity implements FileChooserFragment.FileSelectedInterface,
                                                                NavigationView.OnNavigationItemSelectedListener {
    static String CLASS_TAG = "com.mkehoe.videoanalyzer.MainActivity";

    private static int PRELOAD_THUMB_COUNT = -1;

    private FileChooserFragment mFileChooser = null;
    private VideoSeekerFragment mVideoSeekerFragment = null;
    private MediaInfoFragment mMediaInfoFragment = null;
    private NavigationView mNavigationView;


    private DrawerLayout mDrawerLayout;
    private ActionBarDrawerToggle mDrawerToggle;
    private Menu mMenu;

    //public ByteBuffer [] m_ByteBufferArr;
    public Bitmap[] m_BitmapArr;

    private static Context mContext;

    // Jni methods
    public native MediaInfo OpenFile(String url);
    public native int GetThumbnailFrames(int width, int height, int frameCount);
    public native boolean CloseFile();

    /** Load jni .so on initialization */
    static {
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avfilter-6");

        System.loadLibrary("ffmpegjni");
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        mContext = this;

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);

        mNavigationView = (NavigationView) findViewById(R.id.nav_view);

        mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout, R.string.drawer_open, R.string.drawer_close) {
            @Override
            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
                invalidateOptionsMenu();
            }

            @Override
            public void onDrawerClosed(View drawerView) {
                super.onDrawerClosed(drawerView);
                Log.d(CLASS_TAG, "onDrawerClosed: " + getTitle());
                invalidateOptionsMenu();
            }
        };

        mDrawerLayout.setDrawerListener(mDrawerToggle);
        mNavigationView.setNavigationItemSelectedListener(this);
        mNavigationView.setItemIconTintList(null);

        loadFileChooserFragment();

    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (mDrawerToggle.onOptionsItemSelected(item)) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        mDrawerToggle.syncState();
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        mMenu = menu;
        if( menu != null ) {
            mMenu.setGroupVisible(R.id.action_menu_main_group, false);
            mMenu.findItem(R.id.video_info_item).setEnabled(false);
            mMenu.findItem(R.id.video_frame_item).setEnabled(false);
        }
        return true;
    }


    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        final int id = item.getItemId();
        //mNavigationView.setCheckedItem(id);
        switch (id) {
            case R.id.new_file_item:
                loadFileChooserFragment();
                break;
            case R.id.video_frame_item:
                loadVideoSeekFragment();
                break;
            case R.id.video_info_item:
                loadMediaInfoFragment();
                break;
            default:
                break;
        }

        mDrawerLayout.closeDrawer(GravityCompat.START);

        return true;
    }

    //callback to be called from C++
    public static void thumbnailProcessedCallback(Bitmap bitmap, FrameInfo frameInfo){
        final Bitmap final_bitmap = bitmap;
        final FrameInfo frame = frameInfo;

        ((MainActivity)mContext).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ((MainActivity) mContext).mVideoSeekerFragment.addThumbnail(final_bitmap, frame);
            }
        });

    }

    private void loadFileChooserFragment(){
        if(mFileChooser == null){
            mFileChooser = new FileChooserFragment();
        }
        mFileChooser.setOnFileSelectedListener(this);

        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        transaction.replace(R.id.fragment_container, mFileChooser);
        transaction.commit();
    }
    private void loadVideoSeekFragment(){
        if(mFileChooser != null){
            mFileChooser.removeOnFileSelectedListener(this);
        }
        if(mVideoSeekerFragment == null) {
            mMediaInfoFragment = new MediaInfoFragment();
            mVideoSeekerFragment = new VideoSeekerFragment();

        }

        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        transaction.replace(R.id.fragment_container, mVideoSeekerFragment);
        transaction.commit();

    }
    private void loadMediaInfoFragment(){
        if(mFileChooser != null){
            mFileChooser.removeOnFileSelectedListener(this);
        }
        if(mMediaInfoFragment == null) {
            Log.e(CLASS_TAG, "mMediaInfoFragment is NULL");
        }

        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        transaction.replace(R.id.fragment_container, mMediaInfoFragment);
        transaction.commit();

    }

    @Override
    public void onFileSelected(String url){
        if(mVideoSeekerFragment != null){
            CloseFile();
            mVideoSeekerFragment = null;
        }

        MediaInfo mediaInfo = OpenFile(url);
        if(mediaInfo!= null) {
            loadVideoSeekFragment();
            mMediaInfoFragment.updateMediaInfo(mediaInfo);
            mMenu.findItem(R.id.video_info_item).setEnabled(false);
            mMenu.findItem(R.id.video_frame_item).setEnabled(false);

            /*m_BitmapArr = new Bitmap[PRELOAD_THUMB_COUNT];

            for(int i = 0; i < m_BitmapArr.length; i++) {
                m_BitmapArr[i] = Bitmap.createBitmap(mediaInfo.getRecommendedThumbWidth(), mediaInfo.getRecommendedThumbHeight(), Bitmap.Config.ARGB_8888);
            }*/

            GetThumbnailFrames( mediaInfo.getRecommendedThumbWidth(),
                    mediaInfo.getRecommendedThumbHeight(),
                    PRELOAD_THUMB_COUNT);
        }
        else {
            Log.e(CLASS_TAG, "Failed to open video file");
        }

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }


}
