#pragma once

void com_server_init(void);
void com_stop(void);
float com_get_y(void);
void com_set_u(float value);
void com_request_get_y(void);
void com_request_set_u(void);
void* com_recieve_UDP(void* args);
void* com_transmit_UDP(void* args);

