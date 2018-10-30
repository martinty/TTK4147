#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "com.h" 
#include "../network/udp.h"

#define     UDP_PORT        9999
#define     IP              "192.168.0.1"  


 UDPConn* conn;
 int signal;
 float y;

void com_server_init(void){
    conn = udpconn_new(IP, UDP_PORT);
    signal = 0;
    y = 0;
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

void get_msg_from_server(void){
    
    //send get request
    char sendBuf[64];
    sprintf(sendBuf, "GET");    
    udpconn_send(conn, sendBuf);
    
    //recieve get
    char recvBuf[64];
    udpconn_receive(conn, recvBuf, sizeof(recvBuf));
    //printf(recvBuf);
    printf("\n");
    if(recvBuf[0] == 'S'){
        signal = 1;
        get_msg_from_server();
    }
    else{
        char* value;
        value = recvBuf;
        value = value+8;
        y = atof(value);
    }
}

float com_get_y(void){
    get_msg_from_server();
    return y;
}

int com_get_signal(void){
    return signal;
}


void com_set_u(float value){
    char sendBuf[64];
    sprintf(sendBuf, "SET:%f",value);
    udpconn_send(conn, sendBuf);
}


void com_set_signal_ACK(void){
    char sendBuf[64];
    sprintf(sendBuf, "SIGNAL_ACK");    
    udpconn_send(conn, sendBuf);
    signal = 0;
}


