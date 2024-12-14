CC = gcc
CFLAGS = -Wall -g

SRCS = p2.c files.c List.c
OBJS = p2.o files.o List.o
HEADERS = files.h List.h
TARGET = p2

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
