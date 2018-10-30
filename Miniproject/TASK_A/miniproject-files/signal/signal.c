
#include "../com/com.h"
#include "signal.h"

void signal_handler(){
	while(true){
		if(com_get_signal()){
			com_set_signal_ACK();
		}
		wait();
	}
}


void wait(){
	timer = 1000;
	while(timer > 0){
		timer--;
	}
}
