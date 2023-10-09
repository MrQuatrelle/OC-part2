CC = gcc
CFLAGS=-Wall -Wextra
TARGET=program

all:
	$(CC) $(CFLAGS) *.c utils/logging.c -o $(TARGET)

task1:
	$(CC) $(CFLAGS) -D TASK1 *.c utils/logging.c -o $(TARGET)

task2:
	$(CC) $(CFLAGS) -D TASK2 *.c utils/logging.c -o $(TARGET)

task3:
	$(CC) $(CFLAGS) -D TASK3 *.c utils/logging.c -o $(TARGET)

clean:
	rm $(TARGET)
