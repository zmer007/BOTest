#include <jni.h>
#include <string>
#include <fstream>
#include <iostream>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <unistd.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "tag4LGD", __VA_ARGS__)

jobject k_obj_class_loader = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_lgd_api_MainProxy_init2(JNIEnv *env, jclass clazz, jobject ctx) {
  if (k_obj_class_loader != nullptr) {
    return;
  }
  // 初始化 AAsset 对象
  jclass cls_Context = env->FindClass("android/content/Context");
  jmethodID mtd_Ctx_getAM = env->GetMethodID(cls_Context, "getAssets", "()Landroid/content/res/AssetManager;");
  jobject obj_AM = env->CallObjectMethod(ctx, mtd_Ctx_getAM);
  AAssetManager *asset = AAssetManager_fromJava(env, obj_AM);
  AAsset *obf_dex = AAssetManager_open(asset, "classes.obf.dex", AASSET_MODE_BUFFER);
  size_t obf_dex_length = AAsset_getLength(obf_dex);
  LOGE("obf_dex_length: %d", obf_dex_length);
  // 获取缓存位置: context.getCacheDir();
  jclass cls_File = env->FindClass("java/io/File");
  jmethodID mtd_File_getPath = env->GetMethodID(cls_File, "getPath", "()Ljava/lang/String;");
  jmethodID mtd_Ctx_getCD = env->GetMethodID(cls_Context, "getCacheDir", "()Ljava/io/File;");
  jobject obj_cache_file = env->CallObjectMethod(ctx, mtd_Ctx_getCD);
  auto str_opt_dir = (jstring) env->CallObjectMethod(obj_cache_file, mtd_File_getPath);
  std::string dir_path = env->GetStringUTFChars(str_opt_dir, JNI_FALSE);
  LOGE("cache dir: %s", dir_path.c_str());
  std::string obf_path = {dir_path + "/obf_dex"};
  LOGE("cache dir2: %s", dir_path.c_str());
  // 读取 assets obf 文件
  const void *obf_dex_buf = AAsset_getBuffer(obf_dex);
  // 判断文件是否存在，存在则删除
  if (access(obf_path.c_str(), F_OK)) {
    remove(obf_path.c_str());
  }

  FILE *fp;
  fp = fopen(obf_path.c_str(), "w");
  fwrite(obf_dex_buf, 1, obf_dex_length, fp);
  fclose(fp);
  AAsset_close(obf_dex);

  if (access(obf_path.c_str(), F_OK)) {
    LOGE("error: 文件不存在");
    return;
  }

  jclass cls_DCL = env->FindClass("dalvik/system/DexClassLoader");

  jmethodID mtd_Ctx_getCL = env->GetMethodID(cls_Context, "getClassLoader", "()Ljava/lang/ClassLoader;");
  jmethodID mtd_DCL_init = env->GetMethodID(cls_DCL, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");

  jobject obj_cl = env->CallObjectMethod(ctx, mtd_Ctx_getCL);
  jstring str_dex_path = env->NewStringUTF(obf_path.c_str());
  jobject obj_class_loader = env->NewObject(cls_DCL, mtd_DCL_init, str_dex_path, str_opt_dir, (jobject) nullptr, obj_cl);
  k_obj_class_loader = env->NewGlobalRef(obj_class_loader);
  LOGE("class loader: %p, %p", k_obj_class_loader, str_opt_dir);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_lgd_api_MainProxy_getDeviceId2(JNIEnv *env, jclass clazz, jobject ctx) {
  if (k_obj_class_loader == nullptr) {
    LOGE("未初始化或初始化失败");
    return env->NewStringUTF("未初始化或初始化失败");
  }
  jclass cls_Ctx = env->FindClass("android/content/Context");
  jclass cls_CL = env->FindClass("java/lang/ClassLoader");
  jclass cls_Class = env->FindClass("java/lang/Class");
  jclass cls_Method = env->FindClass("java/lang/reflect/Method");
  jmethodID mtd_Class_fn = env->GetStaticMethodID(cls_Class, "forName", "(Ljava/lang/String;)Ljava/lang/Class;");
  jmethodID mtd_Method_setA = env->GetMethodID(cls_Method, "setAccessible", "(Z)V");
  jmethodID mtd_Class_getDM = env->GetMethodID(cls_Class, "getDeclaredMethod", "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;");
  jmethodID mtd_CL_loadC = env->GetMethodID(cls_CL, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
  jmethodID mtd_Method_IV = env->GetMethodID(cls_Method, "invoke", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
  jstring str_cls_main = env->NewStringUTF("com.lgd.simplet.Main");
  jobject obj_cls_main = env->CallObjectMethod(k_obj_class_loader, mtd_CL_loadC, str_cls_main);
  jstring str_getDI = env->NewStringUTF("getDeviceId");
  jstring str_cls_ctx = env->NewStringUTF("android.content.Context");
  jobject obj_Cls_Ctx = env->CallStaticObjectMethod(cls_Class, mtd_Class_fn, str_cls_ctx);
  jobjectArray objAry_cls_ctx = env->NewObjectArray(1, cls_Class, obj_Cls_Ctx);
  jobject obj_mtd_main_getDI = env->CallObjectMethod(obj_cls_main, mtd_Class_getDM, str_getDI, objAry_cls_ctx);
  env->CallVoidMethod(obj_mtd_main_getDI, mtd_Method_setA, JNI_TRUE);
  jobjectArray objAry_obj_ctx = env->NewObjectArray(1, cls_Ctx, ctx);
  return (jstring) env->CallObjectMethod(obj_mtd_main_getDI, mtd_Method_IV, (jobject) nullptr, objAry_obj_ctx);
}