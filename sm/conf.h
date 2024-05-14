// #define DEBUG_PRINT 1
#define QEMU 1

// #define CALC_CONTEXT_SWITCH_OVERHEAD 1
// #define SW_FAULT 1 // simulate fault as control flow modification (enclave.c)
// #define EXPLOIT 1 // PMP_SET with nop and check regs
// #define EXPLOIT_UNMASK 1  // Attack on unmasking process
// #define EXPLOIT_ENC 1  // Attacking on encryption itself

// #define SILENT 1 // Hidden attack info (such as PMP_unset())

// ===== countermeasures =====
// #define MEM_ENC 1  // Memory encryption (countermeasure)
// #define MEM_ENC_ASCON
#define JAM 1  // countermeasure
// #define RND_DELAY 1  // random delay
// #define DUPL 1  // duplication


// ===== Integrity check =======
#if defined(EXPLOIT) && defined(QEMU)
    #undef QEMU
#endif

#if defined(EXPLOIT) && (defined(EXPLOIT_ENC) || defined(EXPLOIT_UNMASK))
    #undef EXPLOIT
#endif

#if (defined(EXPLOIT_UNMASK) || defined(EXPLOIT_ENC)) && !defined(SW_FAULT)
    #define SW_FAULT 1
#endif
