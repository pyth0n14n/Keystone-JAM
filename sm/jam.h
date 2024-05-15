#ifndef JAM_H
#define JAM_H

#include <stdint.h>
#include "conf.h"

#define ARCH_BIT 64
#define NUM_APP 7  // eapp1~6 + happ
#define TYPE 2  // entry, return address
#define NUM_REG 3 // addr_u, pmpcfg0, pmpaddr7

#ifdef JAM
uint64_t toeplitz(uint64_t *data);
uint64_t unmasked_jump_addr(uint64_t *data, uint8_t app_no, uint8_t ret);
#endif

#endif