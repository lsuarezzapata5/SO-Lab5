#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#define NUMTHREADS 3


sem_t synch;
void *s1(void *arg);
void *s2(void *arg);
void *s3(void *arg);

int main(int argc, char *argv[]){
    pthread_t tid[NUMTHREADS];
    int i=0;

    sem_init(&synch,0,0);
    
    pthread_create(&tid[0], NULL, &s3, NULL); 
    pthread_create(&tid[1], NULL, &s2, NULL); 
    pthread_create(&tid[2], NULL, &s1, NULL); 
    
    for(i = 0; i<NUMTHREADS; i++){
        pthread_join(tid[i], NULL);
    } 

    /*Semaforo destroy*/
    sem_destroy(&synch);

    printf("\n Done! \n");
 
    
    return 0;
}

void *s1(void * unused){
   printf("\n nS1 Executing....\n");
   sem_post(&synch);
   return 0;
   
}

void *s2(void * unused){
   printf("\n nS2 waiting....\n");
   sem_post(&synch);
   sem_wait(&synch);

   printf("\n nS2 Executing....\n");
   return 0;
   
}

void *s3(void * unused){
    printf("\n nS3 waiting....\n");
    sem_wait(&synch);
   printf("\n nS3 Executing....\n");

   return 0;
   
}