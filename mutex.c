#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMTHREADS 200
#define MAXCNT 1000
double counter = 0;
pthread_mutex_t lock;
void *counting(void * unused);


int main(int argc, char *argv[]){
    pthread_t tid[NUMTHREADS];
    int i=0;

    if(pthread_mutex_init(&lock, NULL)!=0){
        printf("\n mutex init failed \n");
        return 1;
    }
    for(i = 0; i<NUMTHREADS; i++){
       pthread_create(&tid[i], NULL, &counting, NULL); 
    } 
    for(i = 0; i<NUMTHREADS; i++){
        pthread_join(tid[i], NULL);
    } 

    /*Mutex destroy*/
    pthread_mutex_destroy(&lock);

    printf("\n Counter must be in: %d\n", MAXCNT*NUMTHREADS);
    printf("\n Counter valuea is: %.0f\n", counter);
    
    return 0;
}

void *counting(void * unused){
    int i=0;
        
    pthread_mutex_lock(&lock);
    for (i = 0; i<MAXCNT; i++){
        counter++;
    }    
    pthread_mutex_unlock(&lock);
    return NULL;   
}