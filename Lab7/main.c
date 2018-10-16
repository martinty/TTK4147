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
#include "TaskB.h"
#include "TaskC.h"
#include "TaskD.h"
  
int main(void){
	rt_print_auto_init(1);
  mlockall(MCL_CURRENT | MCL_FUTURE);
	
	/*
	rt_printf("\nTask A:\n");	
	task_A();
	
	rt_printf("\nTask B:\n");	
	task_B();

	rt_printf("\nTask C:\n");	
	task_C();
	

	rt_printf("\nTask D:\n");	
	task_D();
	*/
	rt_printf("\nTask E:\n");	
	task_E();

  return 0;
}
