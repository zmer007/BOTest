#!/usr/bin/env bash
obfDexToolDir='obfDexTool'
obfBuildDir=$obfDexToolDir/build
rm -rdf $obfBuildDir
mkdir -p $obfBuildDir/assets

./gradlew simplet:clean
./gradlew simplet:aR

# 约定 obfDexTool 生成名称
obfName="classes.obf.dex"

# 预处理阶段
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>预处理阶段 start>>>>>>>>>>>>>>>>>>>>>>>>>>"
# 复制原始 classes.jar 文件
cp ./simplet/build/intermediates/aar_main_jar/release/classes.jar $obfBuildDir

# 拆分 classes.jar 文件
#  - classes.preobf.jar 为不包含 api 相关类，用于 BlackObfuscator 混淆
#  - classes.api.jar 为只包含 api 相关类，供 SDK 对外引用
pushd $obfBuildDir
  # 生成 classes.preobf.jar 包
  cp classes.jar classes.preobf.jar
  zip -d classes.preobf.jar "com/lgd/api/*"
  # 生成 classes.api.jar，由于需要回填到 aar 中，文件名保持 classes.jar 不变
  zip -d classes.jar "com/lgd/simplet/*"
popd

# 将 classes.preobf.jar 文件生成原始 classes.dex 文件，待混淆
d8 --release \
  --lib /Users/lgd/Library/Android/sdk/platforms/android-33/android.jar \
  --output $obfBuildDir \
  $obfBuildDir/classes.preobf.jar
echo "<<<<<<<<<<<<<<<<<<<<<<<<<<预处理阶段 finish<<<<<<<<<<<<<<<<<<<<<<<<<<"
# 将 classes.dex 传递 BlackObfuscator 工具，生成混淆后的 dex 文件 classes.obf.dex

echo ">>>>>>>>>>>>>>>>>>>>>>>>>>混淆阶段 start>>>>>>>>>>>>>>>>>>>>>>>>>>"
pushd $obfDexToolDir/bin
./main "2" "$(readlink -f ../..)/$obfBuildDir/classes.dex" "$(readlink -f ../..)/$obfBuildDir/assets/$obfName"
popd
echo "<<<<<<<<<<<<<<<<<<<<<<<<<<混淆阶段 finish<<<<<<<<<<<<<<<<<<<<<<<<<<"

# 替换阶段
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>替换阶段 start>>>>>>>>>>>>>>>>>>>>>>>>>>"
# 获取接入 SDK
cp ./simplet/build/outputs/aar/simplet-release.aar ./$obfBuildDir
pushd $obfBuildDir
# 删除原始 classes.jar 文件
zip -d simplet-release.aar classes.jar
# 填充 classes.api.jar
zip simplet-release.aar classes.jar
# 填充 classes.obf.dex 文件
zip simplet-release.aar assets/*
# 删除其余临时文件
rm -rdf assets
rm classes.*
popd
echo "<<<<<<<<<<<<<<<<<<<<<<<<<<替换阶段 finish<<<<<<<<<<<<<<<<<<<<<<<<<<"



