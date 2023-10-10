CC = gcc
CFLAGS=-Wall -Wextra

SOURCES  := $(wildcard */*.c)
HEADERS  := $(wildcard */*.h)
OBJECTS  := $(SOURCES:.c=.o)

TARGET_EXECS := l1_cache/TASK1 l2_cache/TASK2 l2_cache_2w/TASK3

l1_cache/TASK1: $(TASK1_OBJECTS) $(UTILS_OBJECTS)
    $(CC) $(CFLAGS) -o $@ $^

l2_cache/TASK2: $(TASK2_OBJECTS) $(UTILS_OBJECTS)
    $(CC) $(CFLAGS) -o $@ $^

l2_cache_2w/TASK3: $(TASK3_OBJECTS) $(UTILS_OBJECTS)
    $(CC) $(CFLAGS) -o $@ $^

TASK1_SOURCES := $(wildcard l1_cache/*.c)
TASK2_SOURCES := $(wildcard l2_cache/*.c)
TASK3_SOURCES := $(wildcard l2_cache_2w/*.c)
UTILS_SOURCES  := $(wildcard utils/*.c)

TASK1_OBJECTS := $(TASK1_SOURCES:.c=.o)
TASK2_OBJECTS := $(TASK2_SOURCES:.c=.o)
TASK3_OBJECTS := $(TASK3_SOURCES:.c=.o)
UTILS_OBJECTS  := $(UTILS_SOURCES:.c=.o)

vpath # clears VPATH
vpath %.h $(INCLUDE_DIRS)


all: $(TARGET_EXECS)

l1_cache/TASK1: $(TASK1_OBJECTS) $(UTILS_OBJECTS)
l2_cache/TASK2: $(TASK2_OBJECTS) $(UTILS_OBJECTS)
l2_cache_2w/TASK3: $(TASK3_OBJECTS) $(UTILS_OBJECTS)


