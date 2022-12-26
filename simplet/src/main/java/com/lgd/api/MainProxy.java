package com.lgd.api;

import android.content.Context;
import android.content.res.AssetManager;

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

    public synchronized static void init(Context ctx) {
        init2(ctx);
    }

    static native void init2(Context ctx);

    static native String getDeviceId2(Context ctx);

    public static String getDeviceId(Context ctx) {
        return getDeviceId2(ctx);
    }
}
