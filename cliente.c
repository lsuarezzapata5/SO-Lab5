#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ 27

int main(void){
    int shmid;
    key_t key;
    char *shm, *s;
    
    key = 1234;
    //Obtiene informacion de la ddireccion de memoria compartida, key=identificadore de a zona, tamaño de la zona y perisos explicitos
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0){
        perror("shmget");
        exit(1);
    }
    
    if ((shm = shmat(shmid, NULL, 0)) == (char *) (-1)){
        perror("shmat");
        exit(1);
    }
    
    for(s = shm; *s != 0; s++){
        putchar(*s);
    } 
    putchar('\n');
    
    *shm = '*';
    exit(0);
}
