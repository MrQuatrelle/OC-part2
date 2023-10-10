#include "Cache.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reset_time();

unsigned int get_time();

/****************  RAM memory (byte addressable) ***************/
void access_dram(int, unsigned char*, int);

/*********************** Cache *************************/

void init_cache();
void access_l1(int, unsigned char*, int);

typedef struct cache_line {
    uint8_t valid;
    uint8_t dirty;
    uint32_t tag;
} cache_line_t;

#if !defined(TASK1) && !defined(TASK2) && !defined(TASK3)
typedef struct cache {
    uint32_t init;
    cache_line_t line;
} cache_t;
#else
typedef struct cache {
    uint32_t init;
    cache_line_t lines[L1_NLINES];
} cache_t;
#endif

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t*);

void write(uint32_t, uint8_t*);

