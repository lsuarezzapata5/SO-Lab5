#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ 27

int main(void){
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    
    key = 1234;
    //Crea memoria compartida y verifica q esta se halla creado bn
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0){
        perror("shmget");
        exit(1);
    }
    //Espacio de memoria
    if ((shm = shmat(shmid, NULL, 0)) == (char *) (-1)){
        perror("shmat");
        exit(1);
    }  
    s = shm;
    for(c = 'a'; c<= 'z'; c++){
        *s++ = c;
    } 
    *s = 0;
    while (*shm != '*'){
       sleep(1); 
    } 
   return 0;
}
