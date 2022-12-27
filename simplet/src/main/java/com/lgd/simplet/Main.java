package com.lgd.simplet;

import android.content.Context;
import android.provider.Settings;

/**
 * Description:
 * <p>
 * Created by lgd on 2022/12/20.
 */
public class Main {
    public static String getDeviceId(Context ctx) {
        StringBuilder sb = new StringBuilder();
        String adid = Settings.System.getString(ctx.getContentResolver(), "android_id");
        if (adid.length() > 10) {
            sb.append(adid.charAt(2));
            sb.append(adid.charAt(4));
            sb.append(adid.charAt(5));
            sb.append(adid.charAt(8));
            sb.append(adid.charAt(7));
            sb.append(adid.charAt(1));
        }
        return "loadFromJNI: " + sb;
    }
}
