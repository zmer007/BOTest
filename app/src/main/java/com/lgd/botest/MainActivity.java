package com.lgd.botest;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.lgd.botest.databinding.ActivityMainBinding;

import java.io.File;
import java.io.InputStream;
import java.lang.reflect.Method;

import dalvik.system.DexClassLoader;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "tag4LGD";

    private ActivityMainBinding binding;

    DexClassLoader classLoader;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
    }

    public void loadObfDex(View ignore) {
        try {
            File OutputDir = FileUtils.getCacheDir(getApplicationContext());
            InputStream dexIS = getAssets().open("classes.obf.dex");

            File desFile = new File(getCacheDir(), "out.dex");
            if (!desFile.exists()) {
                desFile.createNewFile();
                FileUtils.copyFiles(this, dexIS, desFile);
            }
            if (classLoader == null) {
                classLoader = new DexClassLoader(desFile.getPath(),
                        OutputDir.getAbsolutePath(), getApplicationInfo().nativeLibraryDir,
                        getClassLoader());
            }
            // 该方法将Class文件加载到内存时,并不会执行类的初始化,直到这个类第一次使用时才进行初始化.该方法因为需要得到一个ClassLoader对象
            Class<?> clzMain = classLoader.loadClass("com.lgd.simplet.Main");
            Method mtdMain_getDeviceId = clzMain.getDeclaredMethod("getDeviceId", Context.class);
            mtdMain_getDeviceId.setAccessible(true);
            String adid = (String) mtdMain_getDeviceId.invoke(null, this);
            Log.d(TAG, "loadObfDex: " + adid);
        } catch (Exception e) {
            Log.e(TAG, "loadObfDex: ", e);
        }
    }

}