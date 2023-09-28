#include "SimpleCache.h"

int main() {

    int value1, value2, clock;

    reset_time();
    init_cache();
    value1 = -1;
    value2 = 0;

    write(1, (unsigned char*)(&value1));

    clock = get_time();
    printf("Time: %d\n", clock);

    read(1, (unsigned char*)(&value2));
    clock = get_time();
    printf("Time: %d\n", clock);

    write(512, (unsigned char*)(&value1));
    clock = get_time();
    printf("Time: %d\n", clock);

    read(512, (unsigned char*)(&value2));
    clock = get_time();
    printf("Time: %d\n", clock);

    return 0;
}
