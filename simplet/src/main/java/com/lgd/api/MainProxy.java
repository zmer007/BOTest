package com.lgd.api;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.InputStream;
import java.lang.reflect.Method;

import dalvik.system.DexClassLoader;

/**
 * Description:
 * <p>
 * Created by lgd on 2022/12/20.
 */
public class MainProxy {
    private static final String TAG = "tag4LGD";
    private static DexClassLoader sClassLoader;

    private MainProxy() {}

    public synchronized static void init(Context ctx) {
        if (sClassLoader == null) {
            try {
                File OutputDir = FileUtils.getCacheDir(ctx.getApplicationContext());
                InputStream dexIS = ctx.getAssets().open("classes.obf.dex");

                File desFile = new File(ctx.getCacheDir(), "out.dex");
                if (!desFile.exists()) {
                    desFile.createNewFile();
                    FileUtils.copyFiles(ctx, dexIS, desFile);
                }
                sClassLoader = new DexClassLoader(desFile.getPath(),
                        OutputDir.getAbsolutePath(),
                        ctx.getApplicationInfo().nativeLibraryDir,
                        ctx.getClassLoader());
            } catch (Exception e) {
                Log.e(TAG, "init: ", e);
            }
        }
    }

    public static String getDeviceId(Context ctx) {
        init(ctx);
        if (sClassLoader == null) {
            return "初始化失败";
        }
        // 该方法将Class文件加载到内存时,并不会执行类的初始化,直到这个类第一次使用时才进行初始化.该方法因为需要得到一个ClassLoader对象
        try {
            Class<?> clzMain = sClassLoader.loadClass("com.lgd.simplet.Main");
            Method mtdMain_getDeviceId = clzMain.getDeclaredMethod("getDeviceId", Context.class);
            mtdMain_getDeviceId.setAccessible(true);
            return (String) mtdMain_getDeviceId.invoke(null, ctx);
        } catch (Exception e) {
            return e.toString();
        }
    }
}
