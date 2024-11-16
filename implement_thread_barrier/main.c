#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>

#define worker_nums 5

typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t  cond;
	int count;
	int max_count;
} ThreadBarrier;

ThreadBarrier my_barrier;

void thread_barrier_init(ThreadBarrier *barrier, int count){
	barrier->count = 0;
	barrier->max_count = count;
	pthread_mutex_init(&barrier->mutex, NULL);
	pthread_cond_init(&barrier->cond, NULL);

}

void thread_barrier_wait(ThreadBarrier *barrier){
	// Wait for all threads to reach the barrier	
	pthread_mutex_lock(&barrier->mutex);
	barrier->count++;
	
	if(barrier->count == barrier->max_count){
		// Last thread, broadcast all waiting threads
		barrier->count = 0;
		pthread_cond_broadcast(&barrier->cond);
		
	}
	else{
		// Wait for last thread	
		pthread_cond_wait(&barrier->cond, &barrier->mutex); // auto release mutex
	}
	pthread_mutex_unlock(&barrier->mutex);
	

}

void thread_barrier_destroy(ThreadBarrier *barrier){
	pthread_mutex_destroy(&barrier->mutex);
	pthread_cond_destroy(&barrier->cond);
}

void* worker_func(void* arg){
	
	thread_barrier_wait(&my_barrier);
	printf("After waiting, thread %d is running, pid: %d.\n", (int)(long)arg, getpid());
	return NULL;

}
int main(void){
	
	thread_barrier_init(&my_barrier, worker_nums);
	pthread_t workers[worker_nums];
	for(int i = 0; i < worker_nums; i++){
		printf("Starting thead No. %d.\n", i);
		if(pthread_create(&workers[i], NULL, worker_func, (void*)(long) i) != 0){
			fprintf(stderr, "create worker[%d] failed\n", i);
            		exit(1);
		}
	}
	for(int i = 0; i < worker_nums; i++){
		void* res;
		pthread_join(workers[i], &res);
	}
	
	thread_barrier_destroy(&my_barrier);
	return 0;

}
