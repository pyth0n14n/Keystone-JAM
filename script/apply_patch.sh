#!/bin/bash
KEYSTONE_DIR="$PWD/../keystone"
LOCAL_SM_DIR="$PWD/../sm"
LOCAL_KSDRV_DIR="$PWD/../linux-keystone-driver"
KS_SM_DIR="$KEYSTONE_DIR/sm"
KS_KSDRV_DIR="$KEYSTONE_DIR/linux-keystone-driver"

# copy files
cp "$LOCAL_SM_DIR/conf.h" "$KS_SM_DIR/src/"
cp "$LOCAL_SM_DIR/jam.c" "$KS_SM_DIR/src/"
cp "$LOCAL_SM_DIR/jam.h" "$KS_SM_DIR/src/"
cp "$LOCAL_KSDRV_DIR/conf.h" "$KS_KSDRV_DIR/"

# apply patches
cd $KS_SM_DIR
patch -p1 < "$LOCAL_SM_DIR/sm.patch"
cd opensbi
patch -p1 < "$LOCAL_SM_DIR/opensbi.patch"

cd $KS_KSDRV_DIR
patch -p1 < "$LOCAL_KSDRV_DIR/linux-keystone-driver.patch"

