#include <stdlib.h>
#include <stdio.h>
#include <sys/dispatch.h>
#include <pthread.h>
#include <unistd.h>

int chID;
int pID;
int serverInit = 0;

int set_priority(int priority){
    int policy;
    struct sched_param param;

    if(priority < 1 || priority > 63){
        return -1;
    }

    pthread_getschedparam(pthread_self(), &policy, &param);
    param.sched_priority = priority;
    return pthread_setschedparam(pthread_self(), policy, &param);
}

int get_priority(){
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    return param.sched_curpriority;
}


void* client_handler(void* args){
	while(!serverInit){}
	int priority = *(int*) args;
	set_priority(priority);
	int coID = ConnectAttach(0, pID, chID, 0, 0);
	void *smsg[64];
	void *rmsg[64];
	printf("Priority: %d -- Client start -- coID:%d\n", get_priority(), coID);
	smsg[0] = "task A";
	MsgSend(coID, &smsg, 64, &rmsg, 64);
	printf("Priority: %d -- Client: Msg sent and ACK\n", get_priority());
	ConnectDetach(coID);
	printf("Priority: %d -- Client: Connecting detached\n", get_priority());

}

void* server_handler(void* args){
	chID = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);
	pID = getpid();
	void *smsg[64];
	struct _msg_info info;
	serverInit = 1;
	set_priority(4);
	printf("Server start\n");
	while(1){
		printf("server pri = %d \n",get_priority());
		int rcvID = MsgReceive(chID, &smsg, 64, &info);
		printf("server pri = %d \n",get_priority());

		if (rcvID > 0){
			printf("Server: Recived msg -- coID: %d\n",info.coid);
			MsgReply(rcvID, 0, NULL, 0);
			printf("Server: ACK sent\n");
		}
	}
}

int main(int argc, char *argv[]) {
	set_priority(63);

	int priority[4] = {2, 3, 6, 7};
	pthread_t client[4];
	pthread_t server;

	pthread_create(&server, NULL, server_handler, NULL);

	int i = 0;
	while (i < 4){
		pthread_create(&client[i], NULL, client_handler, &priority[i]);
		i++;
	}

	i = 0;
	while (i < 4){
		pthread_join(client[i], NULL);
		i++;
	}
	pthread_join(server, NULL);

	return EXIT_SUCCESS;
}


