#!/bin/sh
KEYSTONE_DIR="$PWD/../keystone"
SAVE_DIR="$KEYSTONE_DIR/build/my-example"
SDK_DIR="$KEYSTONE_DIR/sdk"
BUILD_DIR="$PWD/../build"
EYRIE_DIR="$SAVE_DIR/build/hello-native/runtime/src/eyrie-hello-native-eyrie"
CUR_DIR=$PWD

mkdir -p $SAVE_DIR

cp -rf "$SDK_DIR/examples/hello-native" "$SAVE_DIR/"
cp -f "$SDK_DIR/examples/CMakeLists.txt" "$SAVE_DIR/"
cp -f "$SDK_DIR/macros.cmake" "$SAVE_DIR/"

# remake a bit for 1st compile
sed -i 's/include(..\/macros.cmake)/include(macros.cmake)/' "$SAVE_DIR/CMakeLists.txt"
sed -i '40s/.*/ /' "$SAVE_DIR/CMakeLists.txt"
sed -i '42s/.*/ /' "$SAVE_DIR/CMakeLists.txt"
sed -i '1i cmake_minimum_required(VERSION 3.10)' "$SAVE_DIR/CMakeLists.txt"

cp "$BUILD_DIR/conf.h" "$SAVE_DIR/hello-native/"
cp "$BUILD_DIR/eapp_native.lds" "$SAVE_DIR/hello-native/eapp/"
cp "$BUILD_DIR/host_native.lds" "$SAVE_DIR/hello-native/host/"

mkdir -p "$SAVE_DIR/build"

# 1st patching
cd $KEYSTONE_DIR
patch -p1 < "$BUILD_DIR/my-example.patch"
cd $CUR_DIR

# make & build before copying eyrie files
read -p "Build yourself at $SAVE_DIR/build (cmake .. & make). Finished? (y/N): " yn
case "$yn" in
  [yY]*) ;;
  *) echo "abort" && exit 1;;
esac

cp -f $BUILD_DIR/eyrie/conf.h $EYRIE_DIR/

# 2nd patching
cd $EYRIE_DIR
patch -p1 < "$BUILD_DIR/eyrie/eyrie.patch"

