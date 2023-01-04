#!/usr/bin/env sh
./gradlew clean test:aR
# arm64-v8a / armeabi-v7a / x86 / x86_64
adb push test/build/intermediates/cmake/release/obj/armeabi-v7a/test /data/local/tmp
adb shell chmod +x /data/local/tmp/test
adb shell /data/local/tmp/test