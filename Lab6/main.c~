#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include "io.h"


static const int SIZE = 3;
static const int DISTURBANCE_SIZE = 10;


void taskFunc(int* channel){
  rt_task_set_periodic(NULL, TM_NOW, 1 * 1000 * 1000);
  //unsigned long duration = 10000000000;  // 10 second timeout
  //unsigned long endTime = rt_timer_read() + duration;
  
  while(1){
    if(io_read(channel[0]) == 0){
      io_write(channel[0], 0);
      io_write(channel[0], 1);
    }
    
    /*
    if(rt_timer_read() > endTime){
      rt_printf("Time expired\n");
      rt_task_delete(NULL);
    }
    if(rt_task_yield()){
      rt_printf("Task failed to yield\n");
      rt_task_delete(NULL);
    }
    */
    
    rt_task_wait_period(NULL);
    
  }
}

void* disturbance(void* arg){
  set_cpu(1);
  while(1){
    asm volatile("" ::: "memory");
  }
}

int set_cpu(int cpu_number){
  cpu_set_t cpu;
  CPU_ZERO(&cpu);
  CPU_SET(cpu_number, &cpu);
  return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}
  
int main(void){
  rt_print_auto_init(1);
  mlockall(MCL_CURRENT | MCL_FUTURE);
  
  RT_TASK taskHandler[SIZE];
  const char *taskName[] = {"A", "B", "C"};
  pthread_t disturbanceHandler[DISTURBANCE_SIZE];
  
  for(int i = 0; i < SIZE; i++){
    rt_task_create(&taskHandler[i], taskName[i] , 0, 50, T_CPU(1));
  }
  
  /*
  for (int i = 0; i < DISTURBANCE_SIZE; i++){
    pthread_create(&disturbanceHandler[i], NULL, disturbance, NULL);
  }
  */
  
  int channels[] = {1, 2, 3};
  for(int i = 0; i < SIZE; i++){
    rt_task_start(&taskHandler[i], &taskFunc, &channels[i]);
  }

  
  while(1){}
  return 0;
}