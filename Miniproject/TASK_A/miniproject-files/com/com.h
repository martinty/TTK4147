#pragma once

void com_server_init(void);
void com_stop(void);
void get_msg_from_server(void);
float com_get_y(void);
int com_get_signal(void);
void com_set_u(float value);
void com_set_signal_ACK(void);


/*
(thread) void get_msg_from_server();

float get_y()
bool get_signal()
set_u(int u)
set_signalACK(bool signal);
start()
init()
end()

//int com_send_to_server(char *command,char *string);
*/
