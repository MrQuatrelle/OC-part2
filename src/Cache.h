#include "utils/logging.h"

#define WORD_SIZE (4)                            // in bytes, i.e 32 bit words
#define WORDS_PER_BLOCK (16)                     // in words
#define BLOCK_SIZE (WORDS_PER_BLOCK * WORD_SIZE) // in bytes

#define DRAM_SIZE (1024 * BLOCK_SIZE)            // in bytes

#define L1_NLINES (256)                          // number of lines in L1
#define L2_NLINES (512)                  // number of lines in L2

#define L1_SIZE (L1_NLINES * BLOCK_SIZE)         // in bytes
#define L2_SIZE (L2_NLINES * BLOCK_SIZE) // in bytes

typedef enum {
    MODE_READ,
    MODE_WRITE,
} access_mode;

#define DRAM_READ_TIME (100)
#define L2_READ_TIME (10)
#define L1_READ_TIME (1)

#define DRAM_WRITE_TIME (50)
#define L2_WRITE_TIME (5)
#define L1_WRITE_TIME (1)


#ifdef TWO_WAY_L2

#define L2_NLINES (512)                  // number of lines in L2
#define L2_SIZE (L2_NLINES * BLOCK_SIZE) // in bytes
#define L2_NSETS (2)

#define BYTE_OFFSET (lround(log2(WORD_SIZE)))
#define BLOCK_OFFSET (lround(log2(WORDS_PER_BLOCK)))
#define L2_INDEX_BITS (lround(log2(L2_NLINES / L2_NSETS)))
#define L2_TAG_BITS (32 - L2_INDEX_BITS - BLOCK_OFFSET - BYTE_OFFSET)
#define L2_TAG_OFFSET (BYTE_OFFSET + BLOCK_OFFSET + L2_INDEX_BITS)


#endif
