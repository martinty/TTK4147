#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <native/mutex.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#include "TaskC.h"
#include "TaskB.h"

static const int SIZE = 3;
RT_MUTEX mutWork;
RT_SEM semSynch;

void taskFuncC(void* args){
	
	int channel = *(int*) args;
	rt_sem_p(&semSynch, TM_INFINITE);
	rt_printf("start: Task %d\n",channel);
	switch(channel){

	case 1: // High
		rt_task_sleep(200*1000*1000);
		rt_printf("High: sem wait\n");
		rt_mutex_acquire(&mutWork, TM_INFINITE);		
		rt_printf("High: start work\n");
		busy_wait_us(200*1000);
		rt_printf("High: end work\n");
		rt_mutex_release(&mutWork);
		rt_printf("High: sem unlock\n");
		break;

	case 2: // Medium
		rt_task_sleep(100*1000*1000);
		rt_printf("Medium: start work\n");
		busy_wait_us(500*1000);
		rt_printf("Medium: end work\n");
		break;

	case 3: // Low
		rt_printf("Low: sem wait\n");
		rt_mutex_acquire(&mutWork, TM_INFINITE);
		rt_printf("Low: start work\n");
		busy_wait_us(300*1000);
		rt_printf("Low: end work\n");
		rt_printf("Low: sem unlock\n");
		rt_mutex_release(&mutWork);
		
		break;

 	default:
		rt_printf("Wrong channel!\n");
		break;

	}	
}


void task_C(){
	rt_task_shadow(NULL, "main",1,T_CPU(1) );

	rt_mutex_create(&mutWork,"work");
	rt_sem_create(&semSynch, "synch", 0, S_PRIO);
  RT_TASK taskHandler[SIZE];
  const char *taskName[] = {"High", "Medium", "Low"};
  
  for(int i = 0; i < SIZE; i++){
    rt_task_create(&taskHandler[i], taskName[i] , 0, 20 - i, T_CPU(1) | T_JOINABLE);
  }
    
  int channels[] = {1, 2, 3};
  for(int i = 0; i < SIZE; i++){
    rt_task_start(&taskHandler[i], &taskFuncC, &channels[i]);
  }

	rt_printf("Waiting for synch\n");
	rt_timer_spin(100*1000*1000);
	rt_sem_broadcast(&semSynch);
	

	for (int i = 0; i < SIZE; i++){
		rt_task_join(&taskHandler[i]);
	}

	rt_mutex_delete(&mutWork);
	rt_sem_delete(&semSynch);
}
