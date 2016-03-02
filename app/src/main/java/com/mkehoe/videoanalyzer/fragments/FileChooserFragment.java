package com.mkehoe.videoanalyzer.fragments;

import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ListFragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.mkehoe.videoanalyzer.adapters.FileArrayAdapter;
import com.mkehoe.videoanalyzer.data.FileInfo;
import com.mkehoe.videoanalyzer.R;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Created by mkehoe on 2/27/2016.
 */
public class FileChooserFragment extends ListFragment {
    static String CLASS_TAG = "com.mkehoe.videoanalyzer.fragments.FileChooserFragment";
    private File currentDir;
    private FileArrayAdapter adapter;

    private List<FileSelectedInterface> listeners = new ArrayList<>();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //currentDir = new File(Environment.getExternalStorageDirectory().getPath());
        //populate(currentDir);
    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,Bundle savedInstanceState) {
        View view = super.onCreateView(inflater, container, savedInstanceState);
        currentDir = new File(Environment.getExternalStorageDirectory().getPath());
        populate(currentDir);
        return view;
    }

    private void populate(File f)
    {
        File[]dirs = f.listFiles();
        //this.setTitle("Current Dir: "+f.getName());
        List<FileInfo> dir = new ArrayList<FileInfo>();
        List<FileInfo>fls = new ArrayList<FileInfo>();
        try{
            for(File ff: dirs)
            {
                if(ff.isDirectory())
                    dir.add(new FileInfo(ff.getName(),"Folder",ff.getAbsolutePath()));
                else
                {
                    fls.add(new FileInfo(ff.getName(),"File Size: "+ff.length(),ff.getAbsolutePath()));
                }
            }
        }catch(Exception e) {
            Log.e(CLASS_TAG, "populate exception " + e.toString());
        }
        Collections.sort(dir);
        Collections.sort(fls);
        dir.addAll(fls);
        if(!f.getName().equalsIgnoreCase("sdcard"))
            dir.add(0,new FileInfo("..","Parent Directory",f.getParent()));
        adapter = new FileArrayAdapter(getContext(), R.layout.file_view,dir);
        this.setListAdapter(adapter);
    }

    @Override
    public void onListItemClick(ListView l, View v, int position, long id) {
        // TODO Auto-generated method stub
        super.onListItemClick(l, v, position, id);
        FileInfo o = adapter.getItem(position);
        if(o.getData().equalsIgnoreCase("folder")||o.getData().equalsIgnoreCase("parent directory")){
            currentDir = new File(o.getPath());
            populate(currentDir);
        }
        else
        {
            onFileClick(o);
        }
    }
    private void onFileClick(FileInfo o)
    {
        fireFileSelected(o.getPath());
    }
    public void setOnFileSelectedListener(FileSelectedInterface listener){
        listeners.add(listener);
    }
    public void removeOnFileSelectedListener(FileSelectedInterface listener){
        if(listeners.contains(listener)) {
            listeners.remove(listener);
        }
    }
    private void fireFileSelected(String url){
        for(FileSelectedInterface listener: listeners){
            listener.onFileSelected(url);
        }
    }

    public interface FileSelectedInterface {
        public void onFileSelected(String url);
    }

}
