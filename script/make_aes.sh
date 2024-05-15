#!/bin/bash
KEYSTONE_DIR="../keystone/sm/src"
SAVE_DIR="../sm"
AES_DIR="../tiny-AES-c"
TARGET_H="$SAVE_DIR/aes.h"
TARGET_C="$SAVE_DIR/aes.c"

# ---- aes.h -------
cat "$AES_DIR/aes.h" > $TARGET_H
sed -i "s/#include <stddef.h>/#include <stddef.h>\n#include \"conf.h\"/" $TARGET_H

# ---- aes.c -------
cat "$AES_DIR/aes.c" > $TARGET_C
sed -i "s/#include <string.h>/\/\/ #include <string.h>/" $TARGET_C
sed -i "s/#include \"aes.h\"/#include \"aes.h\"\n#include \"conf.h\"\n#include <stdint.h>\n\n#if defined(MEM_ENC) \&\& !defined(MEM_ENC_ASCON)\nvoid memcpy(uint8_t *dst, const uint8_t *src, size_t size) {\n  int i;\n  for (i = 0; i < size; i++) dst[i] = src[i];\n}/" $TARGET_C

# ---- copy -----
cp -f $TARGET_H $KEYSTONE_DIR/aes.h
cp -f $TARGET_C $KEYSTONE_DIR/aes.c
