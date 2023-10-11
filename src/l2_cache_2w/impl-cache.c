#include "impl-cache.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

uint8_t l1_cache[L1_SIZE];
uint8_t l2_cache[L2_SIZE];
uint8_t dram[DRAM_SIZE];
uint32_t time;
l1_cache_t l1_simple_cache;
l2_cache_t l2_simple_cache;

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
    l1_simple_cache.init = 0;
    l2_simple_cache.init = 0;
}

void access_l2(const uint32_t address, uint8_t* data, access_mode mode) {
    uint32_t tag, set_index, mem_address;
    uint8_t temp_block[BLOCK_SIZE];

    /* init cache */
    if (l2_simple_cache.init == 0) {
        for (int i = 0; i < L2_NLINES; i++) {
            l2_simple_cache.lines[i].valid = false;
            l2_simple_cache.lines[i].lru_counter = ~(0);
        }
        l2_simple_cache.init = 1;
    }

    tag = address >> L2_TAG_OFFSET;

    /* index of l2 cache line */
    set_index =
        (address >> (BYTE_OFFSET + BLOCK_OFFSET)) & ((1 << L2_INDEX_BITS) - 1);

    /* address of beggining of data block */
    mem_address =
        tag << (L2_TAG_OFFSET) | (set_index << (BYTE_OFFSET + BLOCK_OFFSET));

    /* selecting line in set */
    cache_line_t* line = NULL;
    uint32_t line_index;
    uint32_t lru_index = ~(0);
    uint32_t lru_max_seen = 0;

    for (int i = 0; i < L2_NWAYS; i++) {
        line_index = l2_set_to_line_index(set_index) + i;
        cache_line_t* tmp = &(l2_simple_cache.lines[line_index]);

        if (tmp->lru_counter > lru_max_seen) {
            lru_max_seen = tmp->lru_counter;
            lru_index = line_index;
        }

        if (!tmp->valid) {
            line = tmp;
            break;
        }
    }

    /* if all lines are valid, select LRU */
    ALWAYS_ASSERT(lru_index != (uint32_t) ~(0), "Failed to find LRU line");
    line = &(l2_simple_cache.lines[lru_index]);

    /* cache miss */
    if (!line->valid || line->tag != tag) {
        LOG("Cache Read DRAM (+%d)", DRAM_READ_TIME);
        access_dram(mem_address, temp_block, MODE_READ);

        /* write back */
        if ((line->valid) && (line->dirty)) {
            LOG("Write Back. (+%d)", DRAM_WRITE_TIME);
            access_dram(mem_address,
                        &(l2_cache[l2_line_index_to_addr(line_index)]),
                        MODE_WRITE);
        }

        memcpy(&(l2_cache[l2_line_index_to_addr(line_index)]), temp_block,
               BLOCK_SIZE);

        line->valid = true;
        line->tag = tag;
        line->dirty = 0;
    }

    if (mode == MODE_READ) {
        LOG("Cache Hit Read. (+%d)", L2_READ_TIME)
        memcpy(data,
               &(l2_cache[l2_index_plus_word_to_addr(
                   set_index, (address >> BYTE_OFFSET) % WORDS_PER_BLOCK)]),
               WORD_SIZE);

        time += L2_READ_TIME;
    }

    if (mode == MODE_WRITE) {
        LOG("Cache Hit Write. (+%d)", L2_WRITE_TIME)
        memcpy(&(l2_cache[l2_index_plus_word_to_addr(
                   set_index, (address >> BYTE_OFFSET) % WORDS_PER_BLOCK)]),
               data, WORD_SIZE);

        time += L2_WRITE_TIME;
        line->dirty = 1;
    }

    for (int i = 0; i < L2_NLINES; i++) {
        l2_simple_cache.lines[i].lru_counter++;
    }

    line->lru_counter = 0;
}

void access_l1(const uint32_t address, uint8_t* data, access_mode mode) {
    uint32_t tag, l1_line_index, mem_address;
    uint8_t temp_block[BLOCK_SIZE];

    /* init cache */
    if (l1_simple_cache.init == 0) {
        for (int i = 0; i < L1_NLINES; i++) {
            l1_simple_cache.lines[i].valid = false;
        }
        l1_simple_cache.init = 1;
    }

    tag = address >> L1_TAG_OFFSET;

    /* index of l1 cache line */
    l1_line_index =
        (address >> (BYTE_OFFSET + BLOCK_OFFSET)) & ((1 << L1_INDEX_BITS) - 1);

    /* address of beggining of data block */
    mem_address = tag << (L1_TAG_OFFSET) |
                  (l1_line_index << (BYTE_OFFSET + BLOCK_OFFSET));

    cache_line_t* line = &(l1_simple_cache.lines[l1_line_index]);

    /* cache miss */
    if (!(line->valid) || line->tag != tag) {

        access_l2(mem_address, temp_block, MODE_READ);

        /* write back */
        if ((line->valid) && (line->dirty)) {
            LOG("Write Back. (+%d)", DRAM_WRITE_TIME);
            access_l2(mem_address, &(l1_cache[l1_index_to_addr(l1_line_index)]),
                      MODE_WRITE);
        }

        memcpy(&(l1_cache[l1_index_to_addr(l1_line_index)]), temp_block,
               BLOCK_SIZE);

        line->valid = true;
        line->tag = tag;
        line->dirty = 0;
    }

    if (mode == MODE_READ) {
        LOG("Cache Hit Read. (+%d)", L1_READ_TIME)
        memcpy(data,
               &(l1_cache[l1_index_plus_word_to_addr(
                   l1_line_index, (address >> BYTE_OFFSET) % WORDS_PER_BLOCK)]),
               WORD_SIZE);
        time += L1_READ_TIME;
    }

    if (mode == MODE_WRITE) {
        LOG("Cache Hit Write. (+%d)", L1_WRITE_TIME)
        memcpy(&(l1_cache[l1_index_plus_word_to_addr(
                   l1_line_index, (address >> BYTE_OFFSET) % WORDS_PER_BLOCK)]),
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
