CC = gcc
CFLAGS = -std=c11 -O2 -Wall -pthread

TARGET = main
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

#SRCS = samplecode.c
#OBJS = samplecode.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET)
