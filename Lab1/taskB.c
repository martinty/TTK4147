#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <x86intrin.h>
#include <stdint.h>

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

void latency_times(){
    struct tms buf;
    times(&buf);
    for(int i = 0; i < 10*1000*1000;i++){
    	times(&buf);
    }
    printf("%s%ld\n","nsec: ", buf.tms_stime); // buf.tms_stime er i hundredeler
}


void latency_clock_get_time(){
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	struct timespec before = now;
	for(int i = 0; i < 10*1000*1000;i++){
    	clock_gettime(CLOCK_MONOTONIC, &now);
    }
    struct timespec diff = timespec_sub(now, before);
    printf("%s%ld\n","nsec: ", diff.tv_nsec/(10*1000*1000));
}

void latency_rdtsc(){

	int64_t now;
	int64_t past = __rdtsc();

	for(int i = 0; i < 10*1000*1000;i++){
    	now = __rdtsc();
    }

	printf("%s%lld\n","nsec: ",(now-past)/(10*1000*10*266));
}


void latency_times_resolution(){
	int ns_max = 50;
	int histogram[ns_max];
	memset(histogram, 0, sizeof(int)*ns_max);
	struct tms buf;
	clock_t t1, t2;
	int ns;
    for(int i = 0; i < 10*1000*1000;i++){
    	times(&buf);
    	t1 = buf.tms_stime;
    	times(&buf);
    	t2 = buf.tms_stime;
    	ns = t2*10*1000*1000 - t1*10*1000*1000;
    	if(ns >= 0 && ns < ns_max){
      		histogram[ns]++;
    	}

    }
    for(int i = 0; i < ns_max; i++){
    	printf("%d\n", histogram[i]);
	}
}


void latency_clock_get_time_resolution(){
	int ns_max = 50;
	int histogram[ns_max];
	memset(histogram, 0, sizeof(int)*ns_max);
	struct timespec t1, t2;
	int ns;

	for(int i = 0; i < 10*1000*1000;i++){
    	clock_gettime(CLOCK_MONOTONIC, &t1);
    	clock_gettime(CLOCK_MONOTONIC, &t2);
    	ns = t2.tv_nsec - t1.tv_nsec;
		if(ns >= 0 && ns < ns_max){
      		histogram[ns]++;
    	}
    }
    for(int i = 0; i < ns_max; i++){
    	printf("%d\n", histogram[i]);
	}
}


void latency_rdtsc_resolution(){
	int ns_max = 50;
	int histogram[ns_max];
	memset(histogram, 0, sizeof(int)*ns_max);
	int64_t now1, now2;
	int ns;

	for(int i = 0; i < 10*1000*1000;i++){
    	now1 = __rdtsc();
    	now2 = __rdtsc();

    	ns = (now2-now1)*100/266;
    	if(ns >= 0 && ns < ns_max){
      		histogram[ns]++;
    	}
    }
	for(int i = 0; i < ns_max; i++){
    	printf("%d\n", histogram[i]);
	}
}





int main(void){
	//latency_times();
	//latency_clock_get_time();
	//latency_rdtsc();
	//latency_rdtsc_resolution();
	//latency_times_resolution();
	latency_clock_get_time_resolution();

	// ./taskB | gnuplot -p -e "plot '<cat' with boxes"
}