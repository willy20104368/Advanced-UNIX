#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(){
	const char *path = "./sample.txt";
	int fd = open(path, O_RDWR);
	//find string length
	int len;
	//set pointer to begin
	lseek(fd, 14, SEEK_SET);
	
	char* read_buf = (char*)malloc(sizeof(char*)*40);
	
	int n_read;
	n_read = read(fd, read_buf, 8);
	printf("%s\n", read_buf);
	
	char* write_buf = "NTHU student.";
	lseek(fd, 14, SEEK_SET);
	write(fd, write_buf, strlen(write_buf));
	len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	n_read = read(fd, read_buf, len);
	printf("%s, len:%d\n", read_buf, n_read);
	
	return 0;
}
