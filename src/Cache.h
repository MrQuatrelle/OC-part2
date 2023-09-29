#include "utils/logging.h"
#include "Config.h"

#ifdef TASK1
#define ONE_WAY_L1
#endif

#ifdef TASK2
#define ONE_WAY_L1
#define ONE_WAY_L2
#endif

#ifdef TASK3
#define ONE_WAY_L1
#define TWO_WAY_L2
#endif

#ifdef ONE_WAY_L1

#define WORD_SIZE 4                      // in bytes, i.e 32 bit words
#define BLOCK_SIZE 2 * WORD_SIZE         // in bytes
#define DRAM_SIZE 1024 * BLOCK_SIZE      // in bytes
#define L1_NLINES 64                     // number of lines in L1
#define L1_SIZE (L1_NLINES) * BLOCK_SIZE // in bytes
#define L2_SIZE 512 * BLOCK_SIZE         // in bytes

typedef enum {
    MODE_READ,
    MODE_WRITE,
} access_mode;

#define DRAM_READ_TIME 100
#define DRAM_WRITE_TIME 50
#define L2_READ_TIME 10
#define L2_WRITE_TIME 5
#define L1_READ_TIME 1
#define L1_WRITE_TIME 1

#endif /* ifdef ONE_WAY_L1 */

#ifndef ONE_WAY_L1

#define WORD_SIZE 4                 // in bytes, i.e 32 bit words
#define BLOCK_SIZE 2 * WORD_SIZE    // in bytes
#define DRAM_SIZE 1024 * BLOCK_SIZE // in bytes
#define L1_SIZE 1 * BLOCK_SIZE      // in bytes
#define L2_SIZE 512 * BLOCK_SIZE    // in bytes

#define MODE_READ 1
#define MODE_WRITE 0

#define DRAM_READ_TIME 100
#define DRAM_WRITE_TIME 50
#define L2_READ_TIME 10
#define L2_WRITE_TIME 5
#define L1_READ_TIME 1
#define L1_WRITE_TIME 1

#endif /* ifndef ONE_WAY_L1 */
