#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#include "TaskA.h"

static const int SIZE = 2;
RT_SEM semaphore;

void taskFuncA(void* args){
	int channel = *(int*) args;
  rt_sem_p(&semaphore, TM_INFINITE);
  rt_printf("Task: %d \n", channel);
}

void task_A(){
	rt_task_shadow(NULL, "main",1,T_CPU(1) );
  
	rt_sem_create(&semaphore,"sem",0,S_PRIO);
  RT_TASK taskHandler[SIZE];
  const char *taskName[] = {"A1", "A2"};
  
  for(int i = 0; i < SIZE; i++){
    rt_task_create(&taskHandler[i], taskName[i] , 0, 20 - i, T_CPU(1));
  }
    
  int channels[] = {1, 2};
  for(int i = 0; i < SIZE; i++){
    rt_task_start(&taskHandler[i], &taskFuncA, &channels[i]);
  }

	rt_timer_spin(100*1000*1000);
	rt_sem_broadcast(&semaphore);

	for (int i = 0; i < SIZE; i++){
		rt_task_join(&taskHandler[i]);
	}

	rt_sem_delete(&semaphore);
}
