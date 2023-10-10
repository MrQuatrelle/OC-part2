#include "../Cache.h"
#include <stdint.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define BYTE_OFFSET (lround(log2(WORD_SIZE)))
#define BLOCK_OFFSET (lround(log2(WORDS_PER_BLOCK)))
#define L1_INDEX_BITS                                                          \
    (lround(log2(L1_NLINES))) // 2^8 = 256 and thats the number of lines in the
                              // L1 cache.
#define L1_TAG_BITS (32 - L1_INDEX_BITS - BLOCK_OFFSET - BYTE_OFFSET)
#define L1_TAG_OFFSET (BYTE_OFFSET + BLOCK_OFFSET + L1_INDEX_BITS)

void reset_time();

unsigned int get_time();

/****************  RAM memory (byte addressable) ***************/
void access_dram(uint32_t, uint8_t*, access_mode);

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
    cache_line_t lines[L1_NLINES];
} l1_cache_t;

typedef struct l2_cache {
    int init;
    cache_line_t lines[L1_NLINES];
} l2_cache_t;

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t*);

void write(uint32_t, uint8_t*);

/*********************** Macros *************************/
#define l1_index_to_addr(index) (index * BLOCK_SIZE)

#define l1_index_plus_word_to_addr(index, word)                                \
    ((index * BLOCK_SIZE) + (word * WORD_SIZE))
