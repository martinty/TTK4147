#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <pthread.h>

dispatch_t              *dpp;
resmgr_attr_t           resmgr_attr;
dispatch_context_t      *ctp;
resmgr_connect_funcs_t  connect_funcs;
resmgr_io_funcs_t       io_funcs;
iofunc_attr_t           io_attr;

char buf[256];
int value = 0;
int status = 0;

int io_read(resmgr_context_t* ctp, io_read_t* msg, iofunc_ocb_t* ocb);
int io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
void* counter_handler(void* args);

void error(char* s){
	perror(s);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	printf("Start resource manager\n");

	// create dispatch.
	if (!(dpp = dispatch_create())){
		error("Create dispatch");
	}

	// initialize resource manager attributes.
	memset(&resmgr_attr, 0, sizeof(resmgr_attr));
	resmgr_attr.nparts_max = 1;
	resmgr_attr.msg_max_size = 2048;

	// set standard connect and io functions.
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs, _RESMGR_IO_NFUNCS, &io_funcs);
	iofunc_attr_init(&io_attr, S_IFNAM | 0666, 0, 0);

	// override functions
	io_funcs.read = io_read;
	io_funcs.write = io_write;

	// establish resource manager
	if (resmgr_attach(dpp, &resmgr_attr, "/dev/myresource", _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &io_attr) < 0)
		error("Resmanager attach");
	ctp = dispatch_context_alloc(dpp);

	// Make thread
	pthread_t counter;
	pthread_create(&counter, NULL, counter_handler, NULL);

	// wait forever, handling messages.
	while(1){
		if (!(ctp = dispatch_block(ctp))){
			error("Dispatch block");
		}
		dispatch_handler(ctp);
	}
	exit(EXIT_SUCCESS);
}

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb){
	char valueString[256];
	sprintf(valueString,"%i",value);
	if(!ocb->offset){
		SETIOV(ctp->iov, valueString, strlen(valueString));
		_IO_SET_READ_NBYTES(ctp, strlen(valueString));
		// increase the offset (new reads will not get the same data)
		ocb->offset = 1;
		return (_RESMGR_NPARTS(1));
	} else {
		_IO_SET_READ_NBYTES(ctp, 0);
		return (_RESMGR_NPARTS(0));
	}
}

int io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb){
	_IO_SET_WRITE_NBYTES (ctp, msg->i.nbytes);
    resmgr_msgread(ctp, buf, msg->i.nbytes, sizeof(msg->i));
    printf ("Received %d bytes = %s\n", msg -> i.nbytes, buf);

    if(strncmp(buf,"up",2)== 0){
    	status = 1;
    }
    else if(strncmp(buf,"down",4)== 0){
        	status = 2;
        }
    else if(strncmp(buf,"stop",4)== 0){
        	status = 0;
        }

    if (msg->i.nbytes > 0)
           ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;
    return (_RESMGR_NPARTS (0));
}

void* counter_handler(void* args){
	printf("Counter\n");
	useconds_t ms;
	ms = 100 * 1000;
	while(1){
		usleep(ms);
		if(status == 1){
			value++;
		}
		else if(status == 2){
			value--;
		}
	}
}

