#include "utils/logging.h"

#ifdef TASK1
#define ONE_WAY_L1
#endif

#ifdef TASK2
#define ONE_WAY_L1
#define ONE_WAY_L2
#endif

#ifdef TASK3
#define ONE_WAY_L1
#define TWO_WAY_L2
#endif

#ifdef ONE_WAY_L1
#include <math.h>

#define WORD_SIZE (4)                            // in bytes, i.e 32 bit words
#define WORDS_PER_BLOCK (16)                     // in words
#define BLOCK_SIZE (WORDS_PER_BLOCK * WORD_SIZE) // in bytes
#define DRAM_SIZE (1024 * BLOCK_SIZE)            // in bytes
#define L1_NLINES (256)                          // number of lines in L1
#define L1_SIZE (L1_NLINES * BLOCK_SIZE)         // in bytes

#define BYTE_OFFSET (lround(log2(WORD_SIZE)))
#define BLOCK_OFFSET (lround(log2(WORDS_PER_BLOCK)))
#define L1_INDEX_BITS                                                          \
    (lround(log2(L1_NLINES))) // 2^8 = 256 and thats the number of lines in the
                              // L1 cache.
#define L1_TAG_BITS (32 - L1_INDEX_BITS - BLOCK_OFFSET - BYTE_OFFSET)
#define L1_TAG_OFFSET (BYTE_OFFSET + BLOCK_OFFSET + L1_INDEX_BITS)

typedef enum {
    MODE_READ,
    MODE_WRITE,
} access_mode;

#define DRAM_READ_TIME (100)
#define DRAM_WRITE_TIME (50)
#define L1_READ_TIME (1)
#define L1_WRITE_TIME (1)

#endif /* ifdef ONE_WAY_L1 */

#ifdef ONE_WAY_L2

#define L2_NLINES (512)                  // number of lines in L2
#define L2_SIZE (L2_NLINES * BLOCK_SIZE) // in bytes

#define BYTE_OFFSET (lround(log2(WORD_SIZE)))
#define BLOCK_OFFSET (lround(log2(WORDS_PER_BLOCK)))
#define L2_INDEX_BITS                                                          \
    (lround(log2(L2_NLINES))) // 2^8 = 256 and thats the number of lines in the
                              // L2 cache.
#define L2_TAG_BITS (32 - L2_INDEX_BITS - BLOCK_OFFSET - BYTE_OFFSET)
#define L2_TAG_OFFSET (BYTE_OFFSET + BLOCK_OFFSET + L2_INDEX_BITS)

#define L2_READ_TIME (10)
#define L2_WRITE_TIME (5)

#endif /* ifndef ONE_WAY_L2 */
