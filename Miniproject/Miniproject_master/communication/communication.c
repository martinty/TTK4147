#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "communication.h" 
#include "../network/udp.h"

#define     UDP_PORT        9999
#define     IP              "192.168.0.1"  


 UDPConn* conn;
 float y;
 float u;
 int signal;
 int set;
 int get;

void com_server_init(void){
    conn = udpconn_new(IP, UDP_PORT);
    signal = 0;
    y = 0;
    u = 0;
    set = 0;
    get = 0;
    char sendBuf[64];
    sprintf(sendBuf, "START");    
    udpconn_send(conn, sendBuf);
    
}

void com_stop(void){
    char sendBuf[64];
    sprintf(sendBuf, "STOP");    
    udpconn_send(conn, sendBuf);
    udpconn_delete(conn);
}

float com_get_y(void){
    return y;
}

void com_set_u(float value){
    u = value;
}

void com_request_set_u(){
    set = 1;
}

void com_request_get_y(){
    get = 1;
}

void* com_recieve_UDP(void* args){
    char recvBuf[64];
    while(1){ 
        udpconn_receive(conn, recvBuf, sizeof(recvBuf));
        if(recvBuf[0] == 'S'){
            signal = 1;
        }
        else if(recvBuf[0] == 'G'){
            char* value;
            value = recvBuf;
            value = value+8;
            y = atof(value);
            controller_y_is_ready();
        }  
    }
}

void* com_transmit_UDP(void* args){
    char sendBuf[64];
    while(1){
        if(signal){
            sprintf(sendBuf, "SIGNAL_ACK");    
            udpconn_send(conn, sendBuf);
            signal = 0;
        }  
        if(get){
            sprintf(sendBuf, "GET");    
            udpconn_send(conn, sendBuf);
            get = 0;
            }
        if(set){ 
            sprintf(sendBuf, "SET:%f",u);
            udpconn_send(conn, sendBuf);
            set = 0;
        }
    }
}


