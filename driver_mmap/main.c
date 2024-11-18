#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

int main(int argc, char *argv[])
{
    int fd;
    unsigned char *p_map;

    // open device
    fd = open("/dev/mymap", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    // mmap & call my_mmap in driver
    p_map = (unsigned char *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p_map == MAP_FAILED) {
        perror("Failed to mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    // ensure memory map from driver.
    if(p_map != NULL)
    	printf("Kernel string: %s\n", p_map);
    
    // clean
    if (munmap(p_map, PAGE_SIZE) == -1) {
        perror("Failed to munmap");
    }

    close(fd);
    return EXIT_SUCCESS;
}

