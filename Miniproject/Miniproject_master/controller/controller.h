#pragma once

void init_PID(void);
void update_PID(void);
void update_system(void);
void controller_y_is_ready(void);
void* controller_handler(void* args);
