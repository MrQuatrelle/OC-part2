#include "Config.h"

#include "Cache.h"

#ifndef TASK1

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
    unsigned char valid;
    unsigned char dirty;
    unsigned int tag;
} cache_line_t;

typedef struct cache {
    int init;
    cache_line_t lines[L1_NLINES];
} cache_t;

/*********************** Interfaces *************************/

void read(int, unsigned char*);

void write(int, unsigned char*);

#endif /* ifndef ONE_WAY_L1 */
