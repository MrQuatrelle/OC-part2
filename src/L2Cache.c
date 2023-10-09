#include "L2Cache.h"
#include <stdint.h>
#include <stdio.h>

#ifdef ONE_WAY_L2

uint8_t l2_cache[L2_SIZE];
uint8_t dram[DRAM_SIZE];
uint32_t time;
cache_t simple_cache;

/**************** Time Manipulation ***************/
void reset_time() {
    time = 0;
}

uint32_t get_time() {
    return time;
}

/****************  RAM memory (byte addressable) ***************/
void access_dram(uint32_t address, uint8_t* data, access_mode mode) {

    if (address >= DRAM_SIZE - WORD_SIZE + 1) {
        exit(-1);
    }

    if (mode == MODE_READ) {
        memcpy(data, &(dram[address]), BLOCK_SIZE);
        time += DRAM_READ_TIME;
    }

    if (mode == MODE_WRITE) {
        memcpy(&(dram[address]), data, BLOCK_SIZE);
        time += DRAM_WRITE_TIME;
    }
}

/*********************** L2 cache *************************/

void init_cache() {
    simple_cache.init = 0;
}

void access_l2(uint32_t address, uint8_t* data, access_mode mode) {
    uint32_t l2_tag, l2_line_index, mem_address;
    uint8_t temp_block[BLOCK_SIZE];

    /* init cache */
    // NOTE: still don't get it
    if (simple_cache.init == 0) {
        for (int i = 0; i < L2_NLINES; i++) {
            simple_cache.lines[i].valid = false;
        }
        simple_cache.init = 1;
    }

    l2_tag = address >> L2_TAG_OFFSET;

    /* index of l2 cache line */
    l2_line_index = (address >> (BYTE_OFFSET + BLOCK_OFFSET)) & ((1 << L2_INDEX_BITS) - 1);

    /* address of beggining of data block */
    mem_address = l2_tag << ((BYTE_OFFSET + BLOCK_OFFSET) + L2_INDEX_BITS);

    cache_line_t* line = &(simple_cache.lines[l2_line_index]);

    /* cache miss */
    if (!line->valid || line->tag != l2_tag) {
        LOG("Cache Miss (+%d)", DRAM_READ_TIME);
        access_dram(mem_address, temp_block, MODE_READ);

        /* write back */
        if ((line->valid) && (line->dirty)) {
            LOG("Write Back. (+%d)", DRAM_WRITE_TIME);
            #ifdef TASK1
            access_dram(mem_address,
                        &(l2_cache[l2_index_to_addr(l2_line_index)]),
                        MODE_WRITE);
            #else
            // access_l2()
            #endif  

        }

        memcpy(&(l2_cache[0]), temp_block, BLOCK_SIZE);
        line->valid = true;
        line->tag = l2_tag;
        line->dirty = 0;
    }

    if (mode == MODE_READ) {
        LOG("Cache Hit Read. (+%d)", L2_READ_TIME)
        memcpy(data,
               &(l2_cache[l2_index_plus_word_to_addr(
                   l2_line_index, address % WORDS_PER_BLOCK)]),
               WORD_SIZE);
        time += L2_READ_TIME;
    }

    if (mode == MODE_WRITE) {
        LOG("Cache Hit Write. (+%d)", L2_WRITE_TIME)
        memcpy(&(l2_cache[l2_index_plus_word_to_addr(
                   l2_line_index, address % WORDS_PER_BLOCK)]),
               data, WORD_SIZE);
        time += L2_WRITE_TIME;
        line->dirty = 1;
    }
}

void read(uint32_t address, uint8_t* data) {
    access_l1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t* data) {
    access_l1(address, data, MODE_WRITE);
}

#endif