#!/bin/sh
KEYSTONE_DIR="../keystone/sm"
SAVE_DIR="../sm"
ASCON_DIR="../ascon-c"
ASCON_AEAD_DIR=$ASCON_DIR"/crypto_aead/ascon128v12/ref"
TARGET_H="$SAVE_DIR/ascon.h"
TARGET_C="$SAVE_DIR/ascon.c"

# ---- ascon.h -------
echo -e "#ifndef ASCON_H_\n#define ASCON_H_\n\n#include <stdint.h>\n" > $TARGET_H
echo -e "// ========= crypto_aead.h ===========" >> $TARGET_H
cat "$ASCON_DIR/tests/crypto_aead.h" >> $TARGET_H

echo -e "\n// ========= api.h ===========" >> $TARGET_H
cat "$ASCON_AEAD_DIR/api.h" >> $TARGET_H

echo -e "\n// ========= constants.h ===========" >> $TARGET_H
sed -n '6,102p' "$ASCON_AEAD_DIR/constants.h" >> $TARGET_H

echo -e "\n// ========= ascon.h ===========" >> $TARGET_H
sed -n '6,9p' "$ASCON_AEAD_DIR/ascon.h" >> $TARGET_H

echo -e "\n// ========= word.h ===========" >> $TARGET_H
sed -n '7,35p' "$ASCON_AEAD_DIR/word.h" >> $TARGET_H

echo -e "\n// ========= round.h ===========" >> $TARGET_H
sed -n '8,41p' "$ASCON_AEAD_DIR/round.h" >> $TARGET_H

echo -e "\n// ========= permutations.h ===========" >> $TARGET_H
sed -n '11,45p' "$ASCON_AEAD_DIR/permutations.h" >> $TARGET_H

# ABYTES unused
sed -i 's/#define CRYPTO_ABYTES [0-9]*/#define CRYPTO_ABYTES 0/' $TARGET_H
echo -e "#endif /* ASCON_H_ */" >> $TARGET_H

# ---- ascon.c -------
echo -e '#include "conf.h"\n#ifdef MEM_ENC_ASCON\n#include "ascon.h"\n' > $TARGET_C

echo -e "// ========= aead.c ===========" >> $TARGET_C
sed -n '8,180p' "$ASCON_AEAD_DIR/aead.c" >> $TARGET_C
echo -e "#endif" >> $TARGET_C

# ---- copy -----
cp -f $TARGET_H $KEYSTONE_DIR/ascon.h
cp -f $TARGET_C $KEYSTONE_DIR/ascon.c
