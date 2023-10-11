CC = gcc
CFLAGS=-Wall -Wextra

INCLUDE_DIRS := l1_cache l2_cache l2_cache_2w .
INCLUDES = $(addprefix -I , $(INCLUDE_DIRS))

CFLAGS += $(INCLUDES)

SOURCES  := $(wildcard */*.c)
HEADERS  := $(wildcard */*.h)
OBJECTS  := $(SOURCES:.c=.o)

TARGET_EXECS := l1_cache/TASK1 l2_cache/TASK2 l2_cache_2w/TASK3

CLIENT_SOURCE := SimpleProgram.c
TASK1_SOURCES := $(wildcard l1_cache/*.c)
TASK2_SOURCES := $(wildcard l2_cache/*.c)
TASK3_SOURCES := $(wildcard l2_cache_2w/*.c)
UTILS_SOURCES  := $(wildcard utils/*.c)

CLIENT_OBJECTS := SimpleProgram.o
TASK1_OBJECTS := $(TASK1_SOURCES:.c=.o)
TASK2_OBJECTS := $(TASK2_SOURCES:.c=.o)
TASK3_OBJECTS := $(TASK3_SOURCES:.c=.o)
UTILS_OBJECTS  := $(UTILS_SOURCES:.c=.o)

ifneq ($(strip $(DEBUG)), no)
  CFLAGS += -g
endif

ifeq ($(strip $(OPTIM)), no)
  CFLAGS += -O3
else
  CFLAGS += -O0
endif

vpath # clears VPATH
vpath %.h $(INCLUDE_DIRS)

all: $(TARGET_EXECS)

l1_cache/TASK1: $(TASK1_OBJECTS) $(UTILS_OBJECTS) $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
l2_cache/TASK2: $(TASK2_OBJECTS) $(UTILS_OBJECTS) $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
l2_cache_2w/TASK3: $(TASK3_OBJECTS) $(UTILS_OBJECTS) $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS) $(TARGET_EXECS)

.PHONY: clean
