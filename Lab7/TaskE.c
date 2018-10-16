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

#include "TaskE.h"
#include "TaskB.h"




static const int SIZE = 2;
RT_MUTEX mutA;
RT_MUTEX mutB;	
RT_SEM semSynch;

RT_TASK taskHandler[2];

struct mutex {
	RT_MUTEX mut;
	int pri;
};

struct task{
	char name;
	int pri;
};


void taskFuncE(void* args){
	
	int channel = *(int*) args;
	rt_sem_p(&semSynch, TM_INFINITE);
	rt_printf("start: Task %d\n",channel);
	switch(channel){

	case 1: // High
		rt_task_sleep(100*1000*1000);
		rt_printf("High: mutB wait\n");
		rt_mutex_acquire(&mutB, TM_INFINITE);		
		busy_wait_us(100*1000);
		rt_printf("High: mutA wait\n");
		rt_mutex_acquire(&mutA, TM_INFINITE);
		busy_wait_us(200*1000);
		rt_printf("High: mutA unlock\n");
		rt_mutex_release(&mutA);
		rt_printf("High: mutB wait\n");
		rt_mutex_release(&mutB);
		busy_wait_us(100*1000);
		break;

	case 2: // Low

		
		rt_task_set_priority(&taskHandler[1], 20+1);		

		rt_printf("Low: mutA wait\n");
		rt_mutex_acquire(&mutA, TM_INFINITE);
		busy_wait_us(300*1000);
		rt_printf("Low: mutB wait\n");
		rt_mutex_acquire(&mutB, TM_INFINITE);
		busy_wait_us(300*1000);
		rt_printf("Low: mutB unlock\n");
		rt_mutex_release(&mutB);
		rt_printf("Low: mutA unlock\n");
		rt_mutex_release(&mutA);
		busy_wait_us(100*1000);
		break;

 	default:
		rt_printf("Wrong channel!\n");
		break;

	}	
}


void task_E(){
	rt_task_shadow(NULL, "main",1,T_CPU(1) );
	rt_mutex_create(&mutA,"A");
	rt_mutex_create(&mutB,"B");
	rt_sem_create(&semSynch, "synch", 0, S_PRIO);
  
  const char *taskName[] = {"High", "Low"};
  
  for(int i = 0; i < SIZE; i++){
    rt_task_create(&taskHandler[i], taskName[i] , 0, 20 - i, T_CPU(1) | T_JOINABLE);
  }
    
  int channels[] = {1, 2};
  for(int i = 0; i < SIZE; i++){
    rt_task_start(&taskHandler[i], &taskFuncE, &channels[i]);
  }

	rt_printf("Waiting for synch\n");
	rt_timer_spin(100*1000*1000);
	rt_sem_broadcast(&semSynch);
	

	for (int i = 0; i < SIZE; i++){
		rt_task_join(&taskHandler[i]);
	}

	rt_mutex_delete(&mutA);
	rt_mutex_delete(&mutB);
	rt_sem_delete(&semSynch);
}
