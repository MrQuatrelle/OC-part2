#include "L1Cache.h"

#ifdef ONE_WAY_L1

unsigned char l1_cache[L1_SIZE];
unsigned char l2_cache[L2_SIZE];
unsigned char dram[DRAM_SIZE];
unsigned int time;
cache_t simple_cache;

/**************** Time Manipulation ***************/
void reset_time() {
    time = 0;
}

unsigned int get_time() {
    return time;
}

/****************  RAM memory (byte addressable) ***************/
void access_dram(int address, unsigned char* data, access_mode mode) {

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

void access_l1(int address, unsigned char* data, access_mode mode) {
    unsigned int index, tag, mem_address;
    unsigned char temp_block[BLOCK_SIZE];

    /* init cache */
    // NOTE: still don't understand what this is doing
    if (simple_cache.init == 0) {
        for (int i = 0; i < L1_NLINES; i++) {
            simple_cache.lines[i].valid = 0;
        }
        simple_cache.init = 1;
    }

    // NOTE: This gives the index of the block in the cache
    // the 3 least significant bits are the offset
    tag = address >> 3; // Why do I do this?
    int l1_block_index = tag % L1_NLINES;
    cache_line_t* line = &(simple_cache.lines[l1_block_index]);

    // NOTE: the idea I get from this is:
    // since each block is 8 bytes, the tag is the block index
    // and mem_address is the offset in the block (2^3 = 8)
    mem_address = address >> 3; // again this....!
    mem_address = address << 3; // address of the block in memory

    /* access Cache*/
    if (!line->valid || line->tag != tag) { // if block not present - miss
        access_dram(mem_address, temp_block,
                    MODE_READ); // get new block from DRAM

        if ((line->valid) && (line->dirty)) { // line has dirty block
            access_dram(mem_address, &(l1_cache[0]),
                        MODE_WRITE); // then write back old block
        }

        memcpy(&(l1_cache[0]), temp_block,
               BLOCK_SIZE); // copy new block to cache line
        line->valid = 1;
        line->tag = tag;
        line->dirty = 0;
    } // if miss, then replaced with the correct block

    if (mode == MODE_READ) {      // read data from cache line
        if (0 == (address % 8)) { // even word on block
            memcpy(data, &(l1_cache[0]), WORD_SIZE);
        } else { // odd word on block
            memcpy(data, &(l1_cache[WORD_SIZE]), WORD_SIZE);
        }
        time += L1_READ_TIME;
    }

    if (mode == MODE_WRITE) { // write data from cache line
        if (!(address % 8)) { // even word on block
            memcpy(&(l1_cache[0]), data, WORD_SIZE);
        } else { // odd word on block
            memcpy(&(l1_cache[WORD_SIZE]), data, WORD_SIZE);
        }
        time += L1_WRITE_TIME;
        line->dirty = 1;
    }
}

void read(int address, unsigned char* data) {
    access_l1(address, data, MODE_READ);
}

void write(int address, unsigned char* data) {
    access_l1(address, data, MODE_WRITE);
}

#endif /* ifdef ONE_WAY_L1 */
