#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<string.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define BUF_SIZE  1024

// memory stream
struct FileStream{
	char* buffer;
	size_t size;
	size_t pos;
	int pr; // permission read
	int pw; // permission write
};

size_t read_fn(void *fstream, char *buffer, int bytes){
	struct FileStream *fs = (struct FileStream *) fstream;
	if(fs->pr == 0){
		printf("Not readable.\n");
		return -1;
	}
	// check if reach the end of stream
	if(fs->pos >= fs->size){
		return 0;
	}
	// set num of bytes
	size_t read_bytes = (fs->pos + bytes > fs->size) ? (fs->size - fs->pos): fs->pos+bytes;
	memcpy(buffer, fs->buffer+fs->pos, read_bytes);
	fs->pos += bytes;
	return read_bytes;
	
}

size_t write_fn(void *fstream, const char *buffer, int bytes){
	struct FileStream *fs = (struct FileStream *) fstream;
	if(fs->pw == 0){
		printf("Not writable.\n");
		return -1;
	}
	// check if buffer size over the size of filestream
	if(fs->pos + bytes > fs->size){
		fs->buffer = realloc(fs->buffer, fs->pos+bytes);
		if(fs->buffer == NULL){
			printf("Write stream error.\n");
			return 0;
		}
		fs->size = fs->pos + bytes;
			
	}
	memcpy(fs->buffer + fs->pos, buffer, bytes);
	fs->pos += bytes;

	return bytes;
}

off_t seek_fn(void *fstream, off_t offset, int whence){
	struct FileStream *fs = (struct FileStream *) fstream;
	switch(whence){
		case SEEK_SET:
			if(offset < 0 || fs->pos + offset > fs->size){
				printf("Exceed the size of filestream\n");
				return -1;
			}
			fs->pos = offset;
			break;
		
		case SEEK_CUR:
			if(fs->pos + offset < 0 || fs->pos + offset > fs->size){
				printf("Exceed the size of filestream\n");
				return -1;
			}
			fs->pos += offset;
			break;
			
		case SEEK_END:
			if(offset > 0 || fs->size + offset < 0){
				printf("Exceed the size of filestream\n");
				return -1;
			}
			// return the end of filestream
			fs->pos = fs->size;
			break;
		default:
			printf("Enter SEEK_SET, SEEK_CUR or SEEK_END\n");
			return -1;
		
	}
	return 0;
}

int close_fn(void *fstream){
	struct FileStream *fs = (struct FileStream *) fstream;
	free(fs);
	return 0;
}

struct FileStream *fmemopen_fn(void *buffer, size_t size, const char* mode){
	struct FileStream *fs = malloc(sizeof(struct FileStream));
	if(fs == NULL) return NULL;
	fs->buffer = buffer;
	fs->size = size;
	fs->pos = 0;
	
	fs->pr = 0;
	fs->pw = 0;
	
	memset(fs->buffer, 0, fs->size); // clear filestream buffer
	
	if(strcmp(mode, "r") == 0){
		fs->pr = 1;
	}
	else if(strcmp(mode, "w") == 0){
		fs->pw = 1;
	}
	else if(strcmp(mode, "r+") == 0){
		fs->pr = 1;
		fs->pw = 1;
	}
	else{
		free(fs);
		return NULL;
	}

	return fs;
}

// test function
int main(int argc, char **argv){

	if(argc < 2){
		printf("Enter the testing message: ./main <testing message>\n");
		return -1;
	}
	char buffer[BUF_SIZE];
	char read_buf[BUF_SIZE];
	memset(buffer, 0 , sizeof(buffer));
	memset(read_buf, 0 , sizeof(read_buf));
	
	
	//open filestream
	struct FileStream *fstream = fmemopen_fn(buffer, sizeof(buffer), "r+");
	if(fstream == NULL){
		printf("Failed to open memory stream\n");
		return -1;
	}
	
	// write
	//const char *text = "Hello, this is a testing message";
	const char *text = argv[1];

	if(write_fn(fstream, text, strlen(text)) != strlen(text)){
		printf("FileStream write error.\n");
	}
	
	// seek
	seek_fn(fstream, 7, SEEK_SET);
	if(read_fn(fstream, read_buf, strlen(text)-7) < 0){
		printf("Read error.\n");
	}
	printf("Read from FileStream pos7: %s\n", read_buf);

	memset(read_buf, 0 , sizeof(buffer));

	seek_fn(fstream, 0, SEEK_SET);
	//read
	if(read_fn(fstream, read_buf, strlen(text)) < 0){
		printf("Read error.\n");
	}
	printf("Read from FileStream: %s\n", read_buf);
	
	// close
	close_fn(fstream);
	
	return 0;

}
