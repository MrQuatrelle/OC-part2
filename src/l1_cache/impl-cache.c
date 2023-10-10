#include "impl-cache.h"
#include <stdint.h>
#include <stdio.h>

uint32_t time;
uint8_t l1_cache[L1_SIZE], dram[DRAM_SIZE];
l1_cache_t simple_cache;

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

void init_cache() {
    simple_cache.init = 0;
}

void access_l1(uint32_t address, uint8_t* data, access_mode mode) {
    uint32_t l1_tag, l1_line_index, mem_address;
    uint8_t temp_block[BLOCK_SIZE];

    /* init cache */
    if (simple_cache.init == 0) {
        for (int i = 0; i < L1_NLINES; i++) {
            simple_cache.lines[i].valid = false;
        }
        simple_cache.init = 1;
    }

    l1_tag = address >> L1_TAG_OFFSET;

    /* index of l1 cache line */
    l1_line_index =
        (address >> (BYTE_OFFSET + BLOCK_OFFSET)) & ((1 << L1_INDEX_BITS) - 1);

    /* address of beggining of data block */
    mem_address = l1_tag << ((BYTE_OFFSET + BLOCK_OFFSET) + L1_INDEX_BITS);

    cache_line_t* line = &(simple_cache.lines[l1_line_index]);

    /* cache miss */
    if (!line->valid || line->tag != l1_tag) {

        access_dram(mem_address, temp_block, MODE_READ);

        /* write back */
        if ((line->valid) && (line->dirty)) {
            LOG("Write Back. (+%d)", DRAM_WRITE_TIME);
            access_dram(mem_address,
                        &(l1_cache[l1_index_to_addr(l1_line_index)]),
                        MODE_WRITE);
        }

        memcpy(&(l1_cache[0]), temp_block, BLOCK_SIZE);
        line->valid = true;
        line->tag = l1_tag;
        line->dirty = 0;
    }

    if (mode == MODE_READ) {
        LOG("Cache Hit Read. (+%d)", L1_READ_TIME)
        memcpy(data,
               &(l1_cache[l1_index_plus_word_to_addr(
                   l1_line_index, address % WORDS_PER_BLOCK)]),
               WORD_SIZE);
        time += L1_READ_TIME;
    }

    if (mode == MODE_WRITE) {
        LOG("Cache Hit Write. (+%d)", L1_WRITE_TIME)
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
