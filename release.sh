#!/usr/bin/env bash
./gradlew clean
./gradlew simplet:aR

# 生成原始 classes.dex 文件
d8 \
  --lib /Users/lgd/Library/Android/sdk/platforms/android-33/android.jar \
  --release \
  --output tmp \
  simplet/build/intermediates/aar_main_jar/release/classes.jar

# 将 classes.dex 传递 BlackObfuscator 工具，生成混淆后的 dex 文件 classes.obf.dex

# 获取接入 SDK
cp ./simplet/build/outputs/aar/simplet-release.aar ./tmp

pushd tmp
# 删除原始 classes.jar 文件
zip -d simplet-release.aar classes.jar
# 填充 classes.obf.dex 文件
zip simplet-release.aar assets/*
popd



# 填入混淆后的 classes.obf.dex 文件


