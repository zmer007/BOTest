#!/usr/bin/env bash

rm -rdf tmp
mkdir -p tmp/assets

./gradlew clean
./gradlew simplet:aR

# 处理阶段，脚本需要先执行 ./release.sh 1 ，然后使用 BlackObfuscator 处理文件，然后再执行 ./release.sh 2
#  1: 预处理阶段，将原始生成 SDK 包拆分给 BlackObfuscator 处理
#  2: 替换阶段，将 BlackObfuscator 生成的文件填充到预处理后的 SDK
step=$1
# 约定 BlackObfuscator 生成名称
obfName="classes.obf.dex"

# 预处理阶段
if [ "$step" == "1" ] || [ -z $step ]; then
  echo "开始执行 预处理阶段"
  # 复制原始 classes.jar 文件
  cp ./simplet/build/intermediates/aar_main_jar/release/classes.jar tmp

  # 拆分 classes.jar 文件
  #  - classes.preobf.jar 为不包含 api 相关类，用于 BlackObfuscator 混淆
  #  - classes.api.jar 为只包含 api 相关类，供 SDK 对外引用
  pushd tmp
    # 生成 classes.preobf.jar 包
    cp classes.jar classes.preobf.jar
    zip -d classes.preobf.jar "com/lgd/api/*"
    # 生成 classes.api.jar，由于需要回填到 aar 中，文件名保持 classes.jar 不变
    zip -d classes.jar "com/lgd/simplet/*"
  popd

  # 将 classes.preobf.jar 文件生成原始 classes.dex 文件，待混淆
  d8 --release \
    --lib /Users/lgd/Library/Android/sdk/platforms/android-33/android.jar \
    --output tmp \
    tmp/classes.preobf.jar
fi

# 将 classes.dex 传递 BlackObfuscator 工具，生成混淆后的 dex 文件 classes.obf.dex
if [ ! -f "./tmp/assets/$obfName" ]; then
  echo "文件不存在，请执行 BlackObfuscator 处理"
  exit 1
fi

# 替换阶段
if [ "$step" == "2" ]; then
  echo "开始执行 替换阶段"
  # 获取接入 SDK
  cp ./simplet/build/outputs/aar/simplet-release.aar ./tmp
  pushd tmp
  # 删除原始 classes.jar 文件
  zip -d simplet-release.aar classes.jar
  # 填充 classes.api.jar
  zip simplet-release.aar classes.jar
  # 填充 classes.obf.dex 文件
  zip simplet-release.aar assets/*
  popd
fi




