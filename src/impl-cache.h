#include "Cache.h"
#include <stdint.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ONE_WAY_L1

#define BYTE_OFFSET (lround(log2(WORD_SIZE)))
#define BLOCK_OFFSET (lround(log2(WORDS_PER_BLOCK)))
#define L1_INDEX_BITS (lround(log2(L1_NLINES)))
#define L1_TAG_BITS (32 - L1_INDEX_BITS - BLOCK_OFFSET - BYTE_OFFSET)
#define L1_TAG_OFFSET (BYTE_OFFSET + BLOCK_OFFSET + L1_INDEX_BITS)

#define l1_index_to_addr(index) (index * BLOCK_SIZE)

#define l1_index_plus_word_to_addr(index, word)                                \
    ((index * BLOCK_SIZE) + (word * WORD_SIZE))

#endif /* ifdef ONE_WAY_L1 */

#ifdef ONE_WAY_L2

#define l2_index_to_addr(index) (index * BLOCK_SIZE)

#define L2_INDEX_BITS (lround(log2(L2_NLINES)))
#define L2_TAG_BITS (32 - L2_INDEX_BITS - BLOCK_OFFSET - BYTE_OFFSET)
#define L2_TAG_OFFSET (BYTE_OFFSET + BLOCK_OFFSET + L2_INDEX_BITS)

#define l2_line_index_to_addr(index) (index * BLOCK_SIZE)

#define l2_index_plus_word_to_addr(index, word)                                \
    ((index * BLOCK_SIZE) + (word * WORD_SIZE))

#endif /* ifdef ONE_WAY_L2 */

#ifdef TWO_WAY_L2

#endif /* ifdef TWO_WAY_L2 */

/*********************** Cache *************************/

void init_cache();
void access_l1(uint32_t, uint8_t*, access_mode);

typedef struct cache_line {
    bool valid;
    bool dirty;
    uint32_t tag;
} cache_line_t;

typedef struct cache {
    int init;
#ifdef ONE_WAY_L1
    cache_line_t lines[L1_NLINES];
#else
    cache_line_t line;
#endif /* ifdef ONE_WAY_L1 */
} l1_cache_t;

#if defined(ONE_WAY_L2) || defined(TWO_WAY_L2)

typedef struct l2_cache {
    int init;
    cache_line_t lines[L2_NLINES];
} l2_cache_t;

#endif /* if defined (ONE_WAY_L2) || defined (TWO_WAY_L2) */

/*********************** Interfaces *************************/

void reset_time();
unsigned int get_time();
void read(uint32_t, uint8_t*);
void write(uint32_t, uint8_t*);
