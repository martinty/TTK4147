#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <x86intrin.h>
#include <stdint.h>
#include <sched.h>





void latency_clock_get_time_resolution(){
	int ns_max = 300;
	int histogram[ns_max];
	memset(histogram, 0, sizeof(int)*ns_max);
	struct timespec t1, t2;
	int ns;

	for(int i = 0; i < 10*1000*1000;i++){
    	clock_gettime(CLOCK_MONOTONIC, &t1);
    	sched_yield();
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




int main(void) {
	

	latency_clock_get_time_resolution();

	// ./taskC | gnuplot -p -e "plot '<cat' with boxes"

	return 0;
}