#include <sbi/sbi_console.h>
#include <stdint.h>
#include "jam.h"
#include "conf.h"

// #define DEBUG

#ifdef JAM
// https://labs.cybozu.co.jp/blog/takesako/2006/11/binary_hacks.html
inline uint8_t popcount(uint64_t x)
{
    x = ((x & 0xaaaaaaaaaaaaaaaaUL) >> 1)  + (x & 0x5555555555555555UL);
    x = ((x & 0xccccccccccccccccUL) >> 2)  + (x & 0x3333333333333333UL);
    x = ((x & 0xf0f0f0f0f0f0f0f0UL) >> 4)  + (x & 0x0f0f0f0f0f0f0f0fUL);
    x = ((x & 0xff00ff00ff00ff00UL) >> 8)  + (x & 0x00ff00ff00ff00ffUL);
    x = ((x & 0xffff0000ffff0000UL) >> 16) + (x & 0x0000ffff0000ffffUL);
    x = ((x & 0xffffffff00000000UL) >> 32) + (x & 0x00000000ffffffffUL);
    return x;
}

// uint8_t popcount8(uint8_t x)
// {
//     x = ((x & 0xaa) >> 1) + (x & 0x55);
//     x = ((x & 0xcc) >> 2) + (x & 0x33);
//     x = ((x & 0xfc) >> 4) + (x & 0x0f);
//     return x;
// }

static const uint64_t masked_table[NUM_APP][TYPE] =
{
    //   entry               ret addr
#ifdef QEMU
    {0xffffffffffffffff, 0xf28b3d08d02792ce},  // happ
#else
    {0xffffffffffffffff, 0x327297fddaf2ae6f},  // happ
#endif
    {0xc5c893d291005cac, 0xc5c893d291006a16},  // eapp1
    {0x0000000000000000, 0x0000000000000000},  // eapp2
    {0x0000000000000000, 0x0000000000000000},  // eapp3
    {0x0000000000000000, 0x0000000000000000},  // eapp4
    {0x0000000000000000, 0x0000000000000000},  // eapp5
    {0x0000000000000000, 0x0000000000000000}   // eapp6
};

// TODO: making it macro
uint64_t unmasked_jump_addr(uint64_t *data, uint8_t app_no, uint8_t ret) {
    // data:     addr_u || cfg0 || addr7
    // app_no 0: happ, 1: eapp1, ...
    // ret == 1: entry point / else return address
#ifdef DEBUG
    uint64_t mask = toeplitz(data);
    uint64_t masked = masked_table[app_no][ret];
    uint64_t unmasked = masked ^ mask;

    sbi_printf("addr_u   %016lx\n", data[0]);
    sbi_printf("cfg0     %016lx\n", data[1]);
    sbi_printf("addr7    %016lx\n", data[2]);
    sbi_printf("mask     %016lx\n", mask);
    sbi_printf("masked   %016lx\n", masked);
    sbi_printf("unmasked %016lx\n", unmasked);

    return unmasked;
#else
    return masked_table[app_no][ret] ^ toeplitz(data);
#endif
}

static const uint64_t T[ARCH_BIT][NUM_REG] =
{
{0xdeadbeef12345678, 0xfeedbacc09876543, 0xb1edfc3296ed3966},
{0x6f56df77891a2b3c, 0x7f76dd6604c3b2a1, 0xd8f6fe194b769cb3},
{0xb7ab6fbbc48d159e, 0x3fbb6eb30261d950, 0xec7b7f0ca5bb4e59},
{0x5bd5b7dde2468acf, 0x1fddb7598130eca8, 0x763dbf8652dda72c},
{0xadeadbeef1234567, 0x8feedbacc0987654, 0x3b1edfc3296ed396},
{0x56f56df77891a2b3, 0xc7f76dd6604c3b2a, 0x1d8f6fe194b769cb},
{0x2b7ab6fbbc48d159, 0xe3fbb6eb30261d95, 0x0ec7b7f0ca5bb4e5},
{0x15bd5b7dde2468ac, 0xf1fddb7598130eca, 0x8763dbf8652dda72},
{0x8adeadbeef123456, 0x78feedbacc098765, 0x43b1edfc3296ed39},
{0x456f56df77891a2b, 0x3c7f76dd6604c3b2, 0xa1d8f6fe194b769c},
{0x22b7ab6fbbc48d15, 0x9e3fbb6eb30261d9, 0x50ec7b7f0ca5bb4e},
{0x115bd5b7dde2468a, 0xcf1fddb7598130ec, 0xa8763dbf8652dda7},
{0x08adeadbeef12345, 0x678feedbacc09876, 0x543b1edfc3296ed3},
{0x8456f56df77891a2, 0xb3c7f76dd6604c3b, 0x2a1d8f6fe194b769},
{0xc22b7ab6fbbc48d1, 0x59e3fbb6eb30261d, 0x950ec7b7f0ca5bb4},
{0xe115bd5b7dde2468, 0xacf1fddb7598130e, 0xca8763dbf8652dda},
{0x708adeadbeef1234, 0x5678feedbacc0987, 0x6543b1edfc3296ed},
{0xb8456f56df77891a, 0x2b3c7f76dd6604c3, 0xb2a1d8f6fe194b76},
{0xdc22b7ab6fbbc48d, 0x159e3fbb6eb30261, 0xd950ec7b7f0ca5bb},
{0xee115bd5b7dde246, 0x8acf1fddb7598130, 0xeca8763dbf8652dd},
{0xf708adeadbeef123, 0x45678feedbacc098, 0x76543b1edfc3296e},
{0x7b8456f56df77891, 0xa2b3c7f76dd6604c, 0x3b2a1d8f6fe194b7},
{0xbdc22b7ab6fbbc48, 0xd159e3fbb6eb3026, 0x1d950ec7b7f0ca5b},
{0xdee115bd5b7dde24, 0x68acf1fddb759813, 0x0eca8763dbf8652d},
{0x6f708adeadbeef12, 0x345678feedbacc09, 0x876543b1edfc3296},
{0x37b8456f56df7789, 0x1a2b3c7f76dd6604, 0xc3b2a1d8f6fe194b},
{0x9bdc22b7ab6fbbc4, 0x8d159e3fbb6eb302, 0x61d950ec7b7f0ca5},
{0xcdee115bd5b7dde2, 0x468acf1fddb75981, 0x30eca8763dbf8652},
{0xe6f708adeadbeef1, 0x2345678feedbacc0, 0x9876543b1edfc329},
{0xf37b8456f56df778, 0x91a2b3c7f76dd660, 0x4c3b2a1d8f6fe194},
{0x79bdc22b7ab6fbbc, 0x48d159e3fbb6eb30, 0x261d950ec7b7f0ca},
{0xbcdee115bd5b7dde, 0x2468acf1fddb7598, 0x130eca8763dbf865},
{0x5e6f708adeadbeef, 0x12345678feedbacc, 0x09876543b1edfc32},
{0xaf37b8456f56df77, 0x891a2b3c7f76dd66, 0x04c3b2a1d8f6fe19},
{0x579bdc22b7ab6fbb, 0xc48d159e3fbb6eb3, 0x0261d950ec7b7f0c},
{0xabcdee115bd5b7dd, 0xe2468acf1fddb759, 0x8130eca8763dbf86},
{0xd5e6f708adeadbee, 0xf12345678feedbac, 0xc09876543b1edfc3},
{0x6af37b8456f56df7, 0x7891a2b3c7f76dd6, 0x604c3b2a1d8f6fe1},
{0x3579bdc22b7ab6fb, 0xbc48d159e3fbb6eb, 0x30261d950ec7b7f0},
{0x9abcdee115bd5b7d, 0xde2468acf1fddb75, 0x98130eca8763dbf8},
{0x4d5e6f708adeadbe, 0xef12345678feedba, 0xcc09876543b1edfc},
{0x26af37b8456f56df, 0x77891a2b3c7f76dd, 0x6604c3b2a1d8f6fe},
{0x13579bdc22b7ab6f, 0xbbc48d159e3fbb6e, 0xb30261d950ec7b7f},
{0x89abcdee115bd5b7, 0xdde2468acf1fddb7, 0x598130eca8763dbf},
{0xc4d5e6f708adeadb, 0xeef12345678feedb, 0xacc09876543b1edf},
{0xe26af37b8456f56d, 0xf77891a2b3c7f76d, 0xd6604c3b2a1d8f6f},
{0xf13579bdc22b7ab6, 0xfbbc48d159e3fbb6, 0xeb30261d950ec7b7},
{0x789abcdee115bd5b, 0x7dde2468acf1fddb, 0x7598130eca8763db},
{0x3c4d5e6f708adead, 0xbeef12345678feed, 0xbacc09876543b1ed},
{0x9e26af37b8456f56, 0xdf77891a2b3c7f76, 0xdd6604c3b2a1d8f6},
{0xcf13579bdc22b7ab, 0x6fbbc48d159e3fbb, 0x6eb30261d950ec7b},
{0x6789abcdee115bd5, 0xb7dde2468acf1fdd, 0xb7598130eca8763d},
{0xb3c4d5e6f708adea, 0xdbeef12345678fee, 0xdbacc09876543b1e},
{0x59e26af37b8456f5, 0x6df77891a2b3c7f7, 0x6dd6604c3b2a1d8f},
{0xacf13579bdc22b7a, 0xb6fbbc48d159e3fb, 0xb6eb30261d950ec7},
{0x56789abcdee115bd, 0x5b7dde2468acf1fd, 0xdb7598130eca8763},
{0x2b3c4d5e6f708ade, 0xadbeef12345678fe, 0xedbacc09876543b1},
{0x159e26af37b8456f, 0x56df77891a2b3c7f, 0x76dd6604c3b2a1d8},
{0x8acf13579bdc22b7, 0xab6fbbc48d159e3f, 0xbb6eb30261d950ec},
{0x456789abcdee115b, 0xd5b7dde2468acf1f, 0xddb7598130eca876},
{0xa2b3c4d5e6f708ad, 0xeadbeef12345678f, 0xeedbacc09876543b},
{0xd159e26af37b8456, 0xf56df77891a2b3c7, 0xf76dd6604c3b2a1d},
{0x68acf13579bdc22b, 0x7ab6fbbc48d159e3, 0xfbb6eb30261d950e},
{0x3456789abcdee115, 0xbd5b7dde2468acf1, 0xfddb7598130eca87}
};

uint64_t toeplitz(uint64_t *data) {
  int i, j;
  uint64_t c = 0;
  uint64_t tmp;

  for (i = 0; i < ARCH_BIT; i++) {
    tmp = 0;
    for (j = 0; j < NUM_REG; j++) {
      tmp += popcount(T[i][j] & data[j]);
    }
    c |= (uint64_t)((tmp & 0b1) << (ARCH_BIT - 1 - i));
    // printf("%016lx, %016lx\n", tmp, c);
  }

  return c;
}
#endif