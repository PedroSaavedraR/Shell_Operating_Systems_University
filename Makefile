CC = gcc
CFLAGS = -Wall -g

SRCS = main.c files.c List.c
OBJS = main.o files.o List.o
HEADERS = files.h List.h
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

rebuild: clean all

clean:
	rm -f $(OBJS) $(TARGET)

setuid: $(TARGET)
	chmod 4755 $(TARGET)
