#include <stdio.h>
#include <time.h>

#include "../com/com.h"
#include "../time/time.h"
#include "controller.h"
#include <semaphore.h>


float error;
float integral;
float derivative;
float prevError;

float reference;
float kp;
float ki;
float kd;
float dt;

sem_t sem_y;

void init_PID(){
	reference = 1;
	kp = 10;
	ki = 800;
	kd = 0.01;
	dt = 0.004;
	prevError = 0;
	integral = 0;
}

void update_PID(){
	error = reference - com_get_y();
	integral += error * dt;
	derivative = (error - prevError) / dt;
	prevError = error;
}

void update_system(){
	com_set_u(kp*error + ki*integral + kd*derivative);
}

void controller_y_is_ready(){
    sem_post(&sem_y);
}

void* controll_handler(void* args){
    time_start_timer();
	while(1){
	    com_request_get_y();
	    
	    struct timespec wake_time;
	    clock_gettime(CLOCK_MONOTONIC, &wake_time);
	    struct timespec period = {.tv_sec = 0, .tv_nsec = 4000*1000};
	    
	    sem_wait(&sem_y);
	    
		update_PID();
		update_system();
		com_request_set_u();
		
        if(time_check_timer()){
            if(reference == 1){
                reference = 0;
            }
            else{
                com_stop();
                break;
            }
        }
        
        wake_time = timespec_add(wake_time, period);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wake_time, NULL);    
	}
}















