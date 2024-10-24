CC = gcc
CFLAGS = -Wall 

SRCS = p1.c files.c List.c
OBJS = p1.o files.o List.o
HEADERS = files.h List.h
TARGET = p1

all: $(TARGET)
$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

p1.o: p1.c $(HEADERS)
	$(CC) $(CFLAGS) -c p1.c

files.o: files.c files.h
	$(CC) $(CFLAGS) -c files.c

List.o: List.c List.h
	$(CC) $(CFLAGS) -c List.c

rebuild: clean all

clean:
	rm - $(OBJ) $(TARGET)
