## BlackObfuscator 技术可行性验证

本文对开源项目 [BlackObfuscator](https://github.com/CodingGay/BlackObfuscator) 进行技术可行性验证，验证加固 SDK 可行。

### 1 obf-SDK 生成及使用关键步骤

1. SDK 项目生成 classes.jar
2. classes.jar 经 d8 生成 classes.dex 
3. classes.dex 经 BlackObfuscator 生成 classes.obf.dex
4. 将 classes.obf.dex 放到 SDK assets 目录中
5. SDK 将 classes.obf.dex 文件加载到 cache 目录
6. DexClassLoader 加载 classes.obf.dex 文件
7. 通过反射调用原 SDK 公开方法

### 2 obf-SDK 待解决问题

1. 使用 jni 加载 classes.obf.dex 文件
2. 公开 API 保持不变
3. 包体积 & 运行耗时
4. 如何监控线上异常

### 3 BlackObfuscator 工作原理

