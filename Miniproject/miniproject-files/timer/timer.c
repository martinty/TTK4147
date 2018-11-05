#define _GNU_SOURCE
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "timer.h"

struct timespec realTime;
struct timespec referenceTime;
struct timespec second = {.tv_sec = 1, .tv_nsec = 0};

int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec* request, struct timespec* remain) {
    return syscall(__NR_clock_nanosleep, clock_id, flags, request, remain);
} 


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

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int timespec_cmp(struct timespec lhs, struct timespec rhs){
    if (lhs.tv_sec < rhs.tv_sec)
        return -1;
    if (lhs.tv_sec > rhs.tv_sec)
        return 1;
    return lhs.tv_nsec - rhs.tv_nsec;
}

void timer_start_timer(void){
    clock_gettime(CLOCK_MONOTONIC, &realTime);
    referenceTime = timespec_add(second, realTime);
}

int timer_check_timer(void){
    clock_gettime(CLOCK_MONOTONIC, &realTime);
    if(timespec_cmp(realTime, referenceTime) > 0){
        return 1;
    }
    return 0;   
}
