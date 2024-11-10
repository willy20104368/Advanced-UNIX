#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<strings.h>

#define MAX_LINES 1000
#define MAX_LENGTH 100

// compare two strings
int compare_lines(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// compare two strings, ignoring the case of the characters
int compare_lines_ignore(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

// sort strings
void sort_line(char* lines[], int nums, int i_flag){
	if(i_flag)
		qsort(lines, nums, sizeof(char*), compare_lines_ignore);
	else
		qsort(lines, nums, sizeof(char*), compare_lines);
}

// sorted uniq
void uniq(char* lines[], int nums, int c_flag, int i_flag, int u_flag){
	int i, occur = 1;
	int same;
	for(i = 1; i <= nums; i++){
		// repeated string
		if(i < nums && i_flag)
			same = strcasecmp(lines[i-1], lines[i]);
		else if(i < nums)
			same = strcmp(lines[i-1], lines[i]);
			
		if(i < nums && same == 0){
			occur++;
		}
		else{
			// check if is unique
			if((u_flag && occur==1) || !u_flag){
				//check if print counts
				if(c_flag){
					printf("%d %s", occur, lines[i-1]);
				}
				else{
					printf("%s", lines[i-1]);
				}
			}
			occur = 1;
		}
	
	}
}

// Usage: ./main [-i] [-u] [-c] <filename>
int main(int argc, char **argv){
	
	// flags
	int c_flag = 0;
	int i_flag = 0;
	int u_flag = 0;
	
	// array for input file
	char *lines[MAX_LINES];
	int line_counts = 0;
	
	// not match the format
	if(argc < 2){
		fprintf(stderr, "Usage: %s [-i] [-u] [-c] <filename>\n", argv[0]);
		return -1;
	}
	// open original file
	FILE *fp = fopen(argv[argc-1], "r");
	if(fp==NULL){
		printf("Cannot open file.\n");
		return -1;
	}
	
	//check flags (-i -c -u, -icu)
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
		    for (int j = 1; argv[i][j] != '\0'; j++) {
		        switch (argv[i][j]) {
		            case 'c':
		                c_flag = 1;
		                break;
		            case 'i':
		                i_flag = 1;
		                break;
		            case 'u':
		                u_flag = 1;
		                break;
		            default:
		                fprintf(stderr, "Unknown option: -%c\n", argv[i][j]);
		                abort();
		        }
		    }
		}
	}
	
	// read lines from input file
	char buffer[MAX_LENGTH];
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		// copy to array
		if(strcmp(buffer, "\n")!=0){
			char *dest = malloc(strlen(buffer));
			if(dest != NULL)
				strcpy(dest, buffer);
			lines[line_counts] = dest;
			line_counts++;
		}
		
    	}
    	
    	//clsoe file
    	fclose(fp);
    	
    	sort_line(lines, line_counts, i_flag);
    	
	uniq(lines, line_counts, c_flag, i_flag, u_flag);
	
	for (int i = 0; i < line_counts; i++) {
        	free(lines[i]);
    	}
    
	return 0;

}
