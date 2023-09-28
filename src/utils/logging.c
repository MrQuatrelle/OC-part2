#include "logging.h"
#include <stdio.h>

log_level_t g_level = 2;

void set_log_level(log_level_t level) {
    printf("here\n");
    g_level = level;
}
