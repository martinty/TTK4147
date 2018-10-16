#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

static const int SIZE = 5;
pthread_mutex_t mtx[SIZE];

void* fn(void* args){
    int i1 = (int) args;
    int i2 = i1 + 1;
    if (i2 == SIZE){
        i2 = 0;
    }
    if(i1 == 0){
        int temp = i1;
        i1 = i2;
        i2 = temp;
    }
    while(1){
        printf("%d%s\n", i1+1, ": Thinking" );
        pthread_mutex_lock(&mtx[i1]);
        pthread_mutex_lock(&mtx[i2]);
        printf("%d%s\n", i1+1, ": Eating" );
        pthread_mutex_unlock(&mtx[i1]);
        pthread_mutex_unlock(&mtx[i2]);
    }
    return NULL;
}

int main(){
    pthread_t threadHandle[SIZE];
    for (int i = 0; i < SIZE; i++){
        pthread_mutex_init(&mtx[i], NULL);
    }
    for (int i = 0; i < SIZE; i++){
        pthread_create(&threadHandle[i], NULL, fn, (void*)i);
    }
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threadHandle[i], NULL);
    }
    for (int i = 0; i < SIZE; i++) {
        pthread_mutex_destroy(&mtx[i]);
    }
}
