CC = gcc
CFLAGS = -std=c11 -O2 -Wall

TARGET = main
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
#TARGET = assignment1
#SRCS = assignment1.c
#OBJS = assignment1.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET)
