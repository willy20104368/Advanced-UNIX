#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

	pid_t pid = fork();
	
	if(pid < 0){
		printf("Fork error.\n");
		return -1;
	}
	else if(pid == 0){
		// child process
		printf("Child PID: %d\n", getpid());
		// create a new session
		if(setsid() < 0){
			printf("Create session failed.\n");
			return -1;
		}
		
		// Using 'ps' to verify process details
		char command[100] = "ps -o pid,ppid,pgrp,tpgid ";
		char childpid[10] = "";
		sprintf(childpid, "%d", getpid());
		strcat(command, childpid);
		
		// Execute 'ps'
		if(system(command) < 0){
			printf("ps command error.\n");
			return -1;
		}
		
		
	}
	else{
		// wait for child process
		wait(NULL);
	
	}
	
	return 0;

}
