#!/bin/bash
KEYSTONE_DIR="$PWD/../keystone"
LOCAL_SM_DIR="$PWD/../sm"
LOCAL_KSDRV_DIR="$PWD/../linux-keystone-driver"
KS_SM_DIR="$KEYSTONE_DIR/sm"
KS_SKDRV_DIR="$KEYSTONE_DIR/linux-keystone-driver"

# copy files
cp -f "$LOCAL_SM_DIR/conf.h" "$KS_SM_DIR/"
cp -f "$LOCAL_SM_DIR/jam.*" "$KS_SM_DIR/"
cp -f "$LOCAL_KSDRV_DIR/conf.h" "$KS_KSDRV_DIR/"

# apply patches
cd $KS_SM_DIR
patch -p1 < "$LOCAL_SM_DIR/sm.patch"
cd opensbi
patch -p1 < "$LOCAL_SM_DIR/opensbi.patch"

cd $KS_KSDRV_DIR
patch -p1 < "$LOCAL_KSDRV_DIR/linux-keystone-driver.patch"

