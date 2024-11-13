CC = gcc
#CFLAGS = -std=c11 -O2 -Wall -D_POSIX_C_SOURCE=200809L # make sure POSIX 2008
CFLAGS = -std=c11 -O2 -Wall

TARGET = main
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET)
