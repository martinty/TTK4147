#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>


static long glob_int = 0;
sem_t semaphore;

// Note the argument and return types: void*
void* fn(void* args){
    long local_int = 0;
    for (int i = 0; i< 10000000;i++){
        local_int ++;
        sem_wait(&semaphore);
        glob_int ++;
        sem_post(&semaphore);
    }
    printf("%s%ld%s%ld\n","glob_int = ",glob_int,"    local_int = ",local_int );
    return NULL;
}

int main(){
    sem_init(&semaphore, 0, 1);

    pthread_t threadHandle;
    pthread_t threadHandle_1;

    pthread_create(&threadHandle, NULL, fn, NULL);
    pthread_create(&threadHandle_1, NULL, fn, NULL);

    pthread_join(threadHandle, NULL);
    pthread_join(threadHandle_1, NULL);
    sem_destroy(&semaphore);

    return 0;
}
