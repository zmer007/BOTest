package com.lgd.api;

/**
 * Description:
 * <p>
 * Created by lgd on 2022/12/20.
 */
public class MainProxy {
    static {
        System.loadLibrary("simplet");
    }

    private MainProxy() {}

    public synchronized static void init() {
        init2();
    }

    static native void init2();

    static native String getDeviceId2();

    public static String getDeviceId() {
        return getDeviceId2();
    }
}
