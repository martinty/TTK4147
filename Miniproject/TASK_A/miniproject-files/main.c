#include "com/com.h"
#include "controller/controller.h"

// sudo chown student:student /export/nfs/miniproject/ -R
// sudo rm miniproject.out 
// sudo cp miniproject.out /export/nfs
// minicom -D /dev/ttyS0 -> Ctrl+A -> Z -> o -> e -> e -> enter -> enter -> exit
// pwd: see your current directory

int main(void){

    init_PID();
    com_server_init();    
    
    controll_handler();
    
    return 0;
}

