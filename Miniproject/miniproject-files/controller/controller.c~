#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#include "../communication/communication.h"
#include "../timer/timer.h"
#include "controller.h"

#define PERIOD 4000*1000 // 4000 us

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

void init_PID(void){
	reference = 1;
	kp = 10;
	ki = 800;
	kd = 0.01;
	dt = 0.004;
	prevError = 0;
	integral = 0;
}

void update_PID(void){
	error = reference - com_get_y();
	integral += error * dt;
	derivative = (error - prevError) / dt;
	prevError = error;
}

void update_system(void){
    float u = kp*error + ki*integral + kd*derivative;
    if (u > 15){
        u = 15;
    }
    else if(u < -15){
        u = -15;
    }
	com_set_u(u);
}

void controller_y_is_ready(void){
    sem_post(&sem_y);
}

void* controller_handler(void* args){
    timer_start_timer();
	while(1){
	    struct timespec wake_time;
	    clock_gettime(CLOCK_MONOTONIC, &wake_time);
	    struct timespec period = {.tv_sec = 0, .tv_nsec = PERIOD};
	    com_request_get_y();
	    sem_wait(&sem_y);
		update_PID();
		update_system();
		com_request_set_u();
        if(timer_check_timer()){
            if(reference == 1){
                reference = 0;
                timer_start_timer();
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


