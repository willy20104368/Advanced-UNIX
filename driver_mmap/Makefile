CC = gcc
CFLAGS = -std=c11 -O2 -Wall

# targets
APP_TARGET = main
DRV_TARGET = mymap.ko

# source files
APP_SRCS = main.c
APP_OBJS = $(APP_SRCS:.c=.o)

# Kernel build variables
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: $(APP_TARGET) $(DRV_TARGET)

$(APP_TARGET): $(APP_OBJS)
	$(CC) $(CFLAGS) -o $(APP_TARGET) $(APP_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build the kernel module (driver)
$(DRV_TARGET): mymap.c
	echo "obj-m += mymap.o" > Kbuild
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f $(APP_OBJS) $(APP_TARGET) Kbuild

test: $(APP_TARGET)
	./$(APP_TARGET)
	
load: $(DRV_TARGET)
	sudo insmod $(DRV_TARGET)

unload:
	sudo rmmod mymap

