#include "Cache.h"
#include <stdint.h>

#ifdef ONE_WAY_L2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reset_time();

unsigned int get_time();

/*********************** Cache *************************/

void init_cache();
void access_l2(uint32_t, uint8_t*, access_mode);

typedef struct cache_line {
    bool valid;
    bool dirty;
    uint32_t tag;
} cache_line_t;

typedef struct cache {
    int init;
    cache_line_t lines[L2_NLINES];
} cache_t;

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t*);

void write(uint32_t, uint8_t*);

/*********************** Macros *************************/
#define l2_index_to_addr(index) (index * BLOCK_SIZE)
#define l2_index_plus_word_to_addr(index, word)                                \
    ((index * BLOCK_SIZE) + (word * WORD_SIZE))

#endif /* ifdef ONE_WAY_L2 */
