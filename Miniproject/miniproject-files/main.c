#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "com/com.h"
#include "controller/controller.h"



// sudo chown student:student /export/nfs/miniproject/ -R
// sudo rm miniproject.out 
// sudo cp miniproject.out /export/nfs
// minicom -D /dev/ttyS0 -> Ctrl+A -> Z -> o -> e -> e -> enter -> enter -> exit
// pwd: see your current directory

int main(void){


    pthread_t controller;
    pthread_t reciever;
    pthread_t transmitter;
    
    init_PID();
    com_server_init();  
    
    pthread_create(&controller, NULL, controll_handler, NULL);
    pthread_create(&reciever, NULL, com_recieve_UDP, NULL);
    pthread_create(&transmitter, NULL, com_transmit_UDP, NULL);

    pthread_join(controller, NULL);
    
    return 0;
}
