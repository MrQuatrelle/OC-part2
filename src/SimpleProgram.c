#include <stdint.h>

#ifdef TASK1
#include "impl-cache.h"
#else
#include "SimpleCache.h"
#endif

int main() {

    int value1, value2, value3, value4, clock;

    reset_time();
    init_cache();
    value1 = -1;
    value3 = (int)((long)(1 << 30) - 1);

    write(512, (uint8_t*)(&value1));

    clock = get_time();
    printf("Time: %d\n", clock);

    read(512, (uint8_t*)(&value2));
    clock = get_time();
    printf("Time: %d, value: %d\n", clock, value2);

    write(513, (uint8_t*)(&value3));
    clock = get_time();
    printf("Time: %d\n", clock);

    read(513, (uint8_t*)(&value4));
    clock = get_time();
    printf("Time: %d, value: %d\n", clock, value4);

    return 0;
}
