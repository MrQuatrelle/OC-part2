#include "Cache.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define l1_index_to_addr(index) (index * BLOCK_SIZE)

#define l1_index_plus_word_to_addr(index, word)                                \
    ((index * BLOCK_SIZE) + (word * WORD_SIZE))

#if defined(ONE_WAY_L2) || defined(TWO_WAY_L2)

#define L2_INDEX_BITS (lround(log2(L2_NLINES)))
#define L2_TAG_BITS (32 - L2_INDEX_BITS - BLOCK_OFFSET - BYTE_OFFSET)
#define L2_TAG_OFFSET (BYTE_OFFSET + BLOCK_OFFSET + L2_INDEX_BITS)

#define l2_index_to_addr(index) (index * BLOCK_SIZE)

#define l2_line_index_to_addr(index) (index * BLOCK_SIZE)

#define l2_index_plus_word_to_addr(index, word)                                \
    ((index * BLOCK_SIZE) + (word * WORD_SIZE))

#endif /* if defined (ONE_WAY_L2) || defined (TWO_WAY_L2) */

#ifdef TWO_WAY_L2

#define L2_NWAYS 2

#define l2_set_to_line_index(set) (set * L2_NWAYS)

#endif /* ifdef TWO_WAY_L2 */

/*********************** Cache *************************/

void init_cache();
void access_l1(uint32_t, uint8_t*, access_mode);

typedef struct cache_line {
    bool valid;
    bool dirty;
    uint32_t tag;
#ifdef TWO_WAY_L2
    uint32_t lru_counter;
#endif
} cache_line_t;

typedef struct cache {
    int init;
#ifdef ONE_WAY_L1
    cache_line_t lines[L1_NLINES];
#else
    cache_line_t line;
#endif
} l1_cache_t;

#if defined(ONE_WAY_L2) || defined(TWO_WAY_L2)

typedef struct l2_cache {
    int init;
    cache_line_t lines[L2_NLINES];
} l2_cache_t;

#endif

/*********************** Interfaces *************************/

void reset_time();
unsigned int get_time();
void read(uint32_t, uint8_t*);
void write(uint32_t, uint8_t*);
