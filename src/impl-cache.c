#include "impl-cache.h"
#include <stdint.h>
#include <stdio.h>

uint8_t dram[DRAM_SIZE];
uint32_t time;
uint8_t l1_cache[L1_SIZE];
l1_cache_t l1_simple_cache;

#if defined(TASK2) || defined(TASK3)
uint8_t l2_cache[L2_SIZE];
l2_cache_t l2_simple_cache;
#endif

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
#if defined(TASK2) || defined(TASK3)
    l2_simple_cache.init = 0;
#endif
}

#if defined(TASK2) || defined(TASK3)

uint8_t l2_cache[L2_SIZE];

void access_l2(uint32_t address, uint8_t* data, access_mode mode) {
    uint32_t tag, mem_address;
    uint8_t temp_block[BLOCK_SIZE];

#ifdef ONE_WAY_L2
    uint32_t l2_line_index;
#elif (defined TWO_WAY_L2)
    uint32_t set_index;
#endif

    /* init cache */
    if (l2_simple_cache.init == 0) {
        for (int i = 0; i < L2_NLINES; i++) {
            l2_simple_cache.lines[i].valid = false;
#ifdef TWO_WAY_L2
            l2_simple_cache.lines[i].lru_counter = 0;
#endif
        }
        l2_simple_cache.init = 1;
    }

    tag = address >> L2_TAG_OFFSET;

    /* index of l2 cache line */
#ifdef ONE_WAY_L2
    l2_line_index =
        (address >> (BYTE_OFFSET + BLOCK_OFFSET)) & ((1 << L2_INDEX_BITS) - 1);
    mem_address = tag << (L2_TAG_OFFSET) |
                  (l2_line_index << (BYTE_OFFSET + BLOCK_OFFSET));
#elif (defined TWO_WAY_L2)
    set_index =
        (address >> (BYTE_OFFSET + BLOCK_OFFSET)) & ((1 << L2_INDEX_BITS) - 1);
    mem_address =
        tag << (L2_TAG_OFFSET) | (set_index << (BYTE_OFFSET + BLOCK_OFFSET));
#endif

    /* address of beggining of data block */

#ifdef ONE_WAY_L2
    cache_line_t* line = &(l2_simple_cache.lines[l2_line_index]);
#else
    /* selecting line in set */
    cache_line_t* line = NULL;
    uint32_t line_index;
    uint32_t lru_index = ~(0);
    uint32_t lru_max_seen = 0;

    for (int i = 0; i < L2_NWAYS; i++) {
        line_index = l2_set_to_line_index(set_index) + i;
        cache_line_t* tmp = &(l2_simple_cache.lines[line_index]);

        /* find LRU */
        if (tmp->lru_counter > lru_max_seen) {
            lru_max_seen = tmp->lru_counter;
            lru_index = line_index;
        }

        /* if line is free or the tag matches */
        if (!tmp->valid || tmp->tag == tag) {
            line = tmp;
            break;
        }
    }

    /* if all lines are valid, select LRU */
    ALWAYS_ASSERT(lru_index != (uint32_t) ~(0), "Failed to find LRU line");
    line = (line) ? line : &(l2_simple_cache.lines[lru_index]);
#endif

    /* cache miss */
    if (!line->valid || line->tag != tag) {
        LOG("Cache Read DRAM (+%d)", DRAM_READ_TIME);
        access_dram(mem_address, temp_block, MODE_READ);

        /* write back */
        if ((line->valid) && (line->dirty)) {
            LOG("Write Back. (+%d)", DRAM_WRITE_TIME);
#ifdef ONE_WAY_L2
            access_dram(mem_address,
                        &(l2_cache[l2_index_to_addr(l2_line_index)]),
                        MODE_WRITE);
#else
            access_dram(mem_address, &(l2_cache[l2_index_to_addr(set_index)]),
                        MODE_WRITE);
#endif
        }

        memcpy(&(l2_cache[0]), temp_block, BLOCK_SIZE);
        line->valid = true;
        line->tag = tag;
        line->dirty = 0;
    }

    if (mode == MODE_READ) {
        LOG("Cache Hit Read. (+%d)", L2_READ_TIME)
#ifdef ONE_WAY_L2
        memcpy(data,
               &(l2_cache[l2_index_plus_word_to_addr(
                   l2_line_index, address % WORDS_PER_BLOCK)]),
               WORD_SIZE);
#else
        memcpy(data,
               &(l2_cache[l2_index_plus_word_to_addr(
                   set_index, (address >> BYTE_OFFSET) % WORDS_PER_BLOCK)]),
               WORD_SIZE);
#endif

        time += L2_READ_TIME;
    }

    if (mode == MODE_WRITE) {
        LOG("Cache Hit Write. (+%d)", L2_WRITE_TIME)
#ifdef ONE_WAY_L2
        memcpy(&(l2_cache[l2_index_plus_word_to_addr(
                   l2_line_index, address % WORDS_PER_BLOCK)]),
               data, WORD_SIZE);
#else
        memcpy(&(l2_cache[l2_index_plus_word_to_addr(
                   set_index, address % WORDS_PER_BLOCK)]),
               data, WORD_SIZE);
#endif
        time += L2_WRITE_TIME;
        line->dirty = 1;
    }

#ifdef TWO_WAY_L2
    for (int i = 0; i < L2_NLINES; i++) {
        l2_simple_cache.lines[i].lru_counter++;
    }
#endif
}

#endif /* if defined(TASK2) || defined(TASK3) */

void access_l1(const uint32_t address, uint8_t* data, access_mode mode) {
    uint32_t tag, l1_line_index, mem_address;
    uint8_t temp_block[BLOCK_SIZE];

    /* init cache */
    if (l1_simple_cache.init == 0) {
#ifdef ONE_WAY_L1
        for (int i = 0; i < L1_NLINES; i++) {
            l1_simple_cache.lines[i].valid = false;
        }
#endif
        l1_simple_cache.init = 1;
    }

    tag = address >> L1_TAG_OFFSET;

    /* index of l1 cache line */
    l1_line_index =
        (address >> (BYTE_OFFSET + BLOCK_OFFSET)) & ((1 << L1_INDEX_BITS) - 1);

    /* address of beggining of data block */
    mem_address = tag << (L1_TAG_OFFSET) |
                  (l1_line_index << (BYTE_OFFSET + BLOCK_OFFSET));

#ifdef ONE_WAY_L1
    cache_line_t* line = &(l1_simple_cache.lines[l1_line_index]);
#else
    cache_line_t* line = &(l1_simple_cache.line);
#endif

    /* cache miss */
    if (!(line->valid) || line->tag != tag) {
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
        line->tag = tag;
        line->dirty = 0;
    }

    if (mode == MODE_READ) {
        LOG("Cache Hit Read. (+%d)", L1_READ_TIME)
#ifdef ONE_WAY_L1
        memcpy(data,
               &(l1_cache[l1_index_plus_word_to_addr(
                   l1_line_index, (address >> BYTE_OFFSET) % WORDS_PER_BLOCK)]),
               WORD_SIZE);
#else
        memcpy(data, &(l1_cache[(address >> BYTE_OFFSET) % WORDS_PER_BLOCK]),
               WORD_SIZE);
#endif
        time += L1_READ_TIME;
    }

    if (mode == MODE_WRITE) {
        LOG("Cache Hit Write. (+%d)", L1_WRITE_TIME)
#ifdef ONE_WAY_L1
        memcpy(&(l1_cache[l1_index_plus_word_to_addr(
                   l1_line_index, (address >> BYTE_OFFSET) % WORDS_PER_BLOCK)]),
               data, WORD_SIZE);
#else
        memcpy(&(l1_cache[(address >> BYTE_OFFSET) % WORDS_PER_BLOCK]), data,
               WORD_SIZE);
#endif
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
