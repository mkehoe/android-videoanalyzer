package com.mkehoe.videoanalyzer.data;

/**
 * Created by mkehoe on 2/27/2016.
 */
public class FileInfo implements Comparable<FileInfo>{
    private String name;
    private String data;
    private String path;

    public FileInfo(String n,String d,String p)
    {
        name = n;
        data = d;
        path = p;
    }

    public String getName()
    {
        return name;
    }

    public String getData()
    {
        return data;
    }

    public String getPath()
    {
        return path;
    }

    @Override
    public int compareTo(FileInfo o) {
        if(this.name != null) {
            return this.name.toLowerCase().compareTo(o.getName().toLowerCase());
        }
        else {
            throw new IllegalArgumentException();
        }
    }
}

