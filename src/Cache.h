typedef enum {
    MODE_READ,
    MODE_WRITE,
} access_mode;

#define WORD_SIZE (4)                            // in bytes, i.e 32 bit words
#define WORDS_PER_BLOCK (16)                     // in words
#define BLOCK_SIZE (WORDS_PER_BLOCK * WORD_SIZE) // in bytes

#define DRAM_SIZE (1024 * BLOCK_SIZE)            // in bytes

#define L1_NLINES (256)                          // number of lines in L1
#define L2_NLINES (512)                          // number of lines in L2

#define L1_SIZE (L1_NLINES * BLOCK_SIZE)         // in bytes
#define L2_SIZE (L2_NLINES * BLOCK_SIZE)         // in bytes

#define DRAM_READ_TIME (100)
#define L2_READ_TIME (10)
#define L1_READ_TIME (1)

#define DRAM_WRITE_TIME (50)
#define L2_WRITE_TIME (5)
#define L1_WRITE_TIME (1)
