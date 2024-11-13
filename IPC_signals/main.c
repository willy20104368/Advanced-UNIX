#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Set nonzero by signal handler
static volatile sig_atomic_t sigflag=0;
static sigset_t newmask; // New signal mask including SIGUSR1, SIGUSR2
static sigset_t oldmask; // Original signal mask
static sigset_t zeromask; // Empty signal set

// Signal handler for SIGUSR1 and SIGUSR2
static void sig_usr(int signo) {
    //printf("Caught signal %d\n", signo);
    sigflag = 1;
}

static void TELL_WAIT(void){
    // Set signal handler for SIGUSR1 and SIGUSR2
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        printf("SIGUSR1 error.\n");
        exit(1);
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        printf("SIGUSR2 error.\n");
        exit(1);
    }

    sigemptyset(&zeromask); // Empty signal set
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    // Block SIGUSR1 and SIGUSR2, and save the original signal mask
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        perror("SIG_BLOCK error");
}

static void TELL_PARENT(void){
    // Notify the parent that the child has completed
    if (kill(getppid(), SIGUSR2) < 0) {
        printf("Signal parent error.\n");
    }
}

static void TELL_CHILD(pid_t pid){
    // Notify the child that the parent has completed
    if (kill(pid, SIGUSR1) < 0) {
        printf("Signal child error.\n");
    }
    //printf("Signal child\n");
}

static void WAIT_PARENT(void){
    // Wait for the parent to signal
    //printf("Child waiting...\n");  // Debugging message
    while (sigflag == 0){
    	//printf("Waiting in while\n");
    	// Wait until signal received
    	if(sigsuspend(&zeromask) != -1){
    		printf("sigsuspend error.\n");
    	}
    }
        
    sigflag = 0;

    // Reset the signal mask
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        printf("SIG_SETMASK error.\n");
    }
    //printf("Child processing...\n");  // Debugging message
}

static void WAIT_CHILD(void){
    // Wait for the child to signal
    //printf("Parent waiting...\n");  // Debugging message
    while (sigflag == 0){
    	//printf("Waiting in while\n");
    	// Wait until signal received
    	if(sigsuspend(&zeromask) != -1){
    		printf("sigsuspend error.\n");
    	}
    }
          
    sigflag = 0;

    // Reset the signal mask
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        printf("SIG_SETMASK error.\n");
    }
    //printf("Parent processing...\n");  // Debugging message
}

static int increment(FILE* const fp){
    int num;
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%d", &num);  // Read current value from the file
    fseek(fp, 0, SEEK_SET);   // Move file pointer to the beginning
    num++;
    fprintf(fp, "%d", num);  // Increment and write back to the file
    fflush(fp);
    return num;
}

int main() {
    /* TODO */
    // 1. Generate a file and initially write the number 0 to it
    // 2. Fork system call, resulting in both the parent and child processes taking turns incrementing the counter within the
    // file by 1 until 100 starts from the child process.
    // 3. For each increment operation, the program prints a message indicating which process, either the parent or the
    // child, is performing the increment.
    
    FILE *fp = fopen("counter.txt", "w+");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return -1;
    }

    fprintf(fp, "%d", 0);  // Start counting from 0
    fflush(fp);

    TELL_WAIT();  // Initialize signal handling

    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork error.\n");
        return -1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child process started\n");
        for (int i = 0; i < 50; ++i) {
            WAIT_PARENT();  // Wait for the parent signal
            int tmp = increment(fp);
            printf("Child value: %d\n", tmp);
            TELL_PARENT();  // Notify parent that the child is done
        }
    }
    else {
        // Parent process
        //sleep(1);
        printf("Parent process started\n");
        for (int j = 0; j < 50; ++j) {
            TELL_CHILD(pid);  // Notify child to proceed
            WAIT_CHILD();  // Wait for the child signal
            int tmp = increment(fp);
            printf("Parent value: %d\n", tmp);
        }
    }

    fclose(fp);
    return 0;
}

