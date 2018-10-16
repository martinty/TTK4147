#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

#include "io.h"

static const int SIZE = 3;
static const int DISTURBANCE_SIZE = 10;

struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void* response_test(int* channel){
	set_cpu(1);
	struct timespec waketime;
	clock_gettime(CLOCK_REALTIME, &waketime);
	struct timespec period = {.tv_sec = 0, .tv_nsec = 1 * 1000 * 1000};
	while(1){
		if(io_read(channel[0]) == 0){
			io_write(channel[0], 0);
			usleep(50);
			io_write(channel[0], 1);
		}
		waketime = timespec_add(waketime, period);
	   	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
	}
}

void* disturbance(void* arg){
	set_cpu(1);
	while(1){
		asm volatile("" ::: "memory");
	}
}

int main(void){
    io_init();
    pthread_t threadHandle[SIZE];
	pthread_t disturbanceHandler[DISTURBANCE_SIZE];
	int channels[3] = {1, 2, 3};
    for (int i = 0; i < SIZE; i++){
        pthread_create(&threadHandle[i], NULL, response_test, &channels[i]);
    }
	for (int i = 0; i < DISTURBANCE_SIZE; i++){
        pthread_create(&disturbanceHandler[i], NULL, disturbance, NULL);
    }
    for (int i = 0; i < SIZE; i++){
        pthread_join(threadHandle[i], NULL);
    }

	for (int i = 0; i < DISTURBANCE_SIZE; i++){
        pthread_join(disturbanceHandler[i], NULL);
    }
	return 0;
}
