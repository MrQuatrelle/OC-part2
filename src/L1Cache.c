#include "L1Cache.h"
#include <stdint.h>
#include <stdio.h>

#ifdef ONE_WAY_L1

uint8_t l1_cache[L1_SIZE];
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

/*********************** L1 cache *************************/

void init_cache() {
    simple_cache.init = 0;
}

void access_l1(uint32_t address, uint8_t* data, access_mode mode) {
    uint32_t mem_address;
    uint8_t temp_block[BLOCK_SIZE];

    /* init cache */
    // NOTE: still don't get it
    if (simple_cache.init == 0) {
        for (int i = 0; i < L1_NLINES; i++) {
            simple_cache.lines[i].valid = false;
        }
        simple_cache.init = 1;
    }

    uint32_t tag = address >> L1_TAG_OFFSET;

    /* index of l1 cache line */
    uint32_t l1_line_index =
        (address >> BLOCK_OFFSET_BITS) & ((1 << L1_INDEX_BITS) - 1);

    printf("address: %x, tag: %x, line index: %x\n", address, tag,
           l1_line_index);

    cache_line_t* line = &(simple_cache.lines[l1_line_index]);

    /* address of beggining of data block */
    mem_address = tag << (BLOCK_OFFSET_BITS + L1_INDEX_BITS);

    /* cache miss */
    if (!line->valid || line->tag != tag) {
        access_dram(mem_address, temp_block, MODE_READ);

        /* write back */
        if ((line->valid) && (line->dirty)) {
            access_dram(mem_address,
                        &(l1_cache[l1_index_to_addr(l1_line_index)]),
                        MODE_WRITE);
        }

        memcpy(&(l1_cache[0]), temp_block, BLOCK_SIZE);
        line->valid = true;
        line->tag = tag;
        line->dirty = 0;
    }

    if (mode == MODE_READ) {
        memcpy(data,
               &(l1_cache[l1_index_plus_word_to_addr(
                   l1_line_index, address % WORDS_PER_BLOCK)]),
               WORD_SIZE);
    }
    time += L1_READ_TIME;

    if (mode == MODE_WRITE) {
        memcpy(&(l1_cache[l1_index_plus_word_to_addr(
                   l1_line_index, address % WORDS_PER_BLOCK)]),
               data, WORD_SIZE);
        time += L1_WRITE_TIME;
        line->dirty = 1;
    }
}

void read(uint32_t address, uint8_t* data) {
    access_l1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t* data) {
    access_l1(address, data, MODE_WRITE);
}

#endif /* ifdef ONE_WAY_L1 */
