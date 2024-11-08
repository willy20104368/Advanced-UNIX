#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define BUF_SIZE 1024

int main(int argc, char **argv){
	// argv[1]: src, argv[2]: dest
	// ./main <src file> <dest file>
	if(argc < 3){
		fprintf(stderr, "Usage %s <source file> <destination file>\n", argv[0]);
		return -1;
	}
	// open source
	int src, dest;
	src = open(argv[1], O_RDONLY);
	if(src < 0){
		perror("Cannot open source file");
		return -1;
	}
	// open dest
	dest = open(argv[2], O_WRONLY | O_CREAT, 00700);
	if(dest < 0){
		perror("Cannot open destination file");
		close(src);
		return -1;
	}
	
	char buffer[BUF_SIZE];
	ssize_t byte_read, byte_write;
	
	// read & write by BUF_SIZE block
	while((byte_read = read(src, buffer, sizeof(buffer))) > 0){
		byte_write = write(dest, buffer, byte_read);
		if(byte_write != byte_read){
			perror("File writing error");
			close(src);
			close(dest);
			return -1;
		}
	}
	
	
	close(src);
	close(dest);
	return 0;
}
