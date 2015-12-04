#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BUFFER 10	/* Número de ranuras en el buffer: Tamaño del buffer */    

void* productor(void* unusued);
int producir_elemento(void);
void insertar_elemento(int *dato);
void* consumidor(void* unusued);
int quitar_elemento(void);
void consumir_elemento(int *dato);

sem_t mutex;
sem_t vacias;
sem_t llenas;
int buffer[MAX_BUFFER];	                  
int posicion_in = 0;   /* Posición donde insertar el elemento */
int posicion_out = 0;  /* Posicion donde extraer un elelemnto */
int contador = 0;      /* Cuenta elementos en el bufer */

int main(void){

    sem_init(&mutex, 0, 1);
    sem_init(&vacias, 0, MAX_BUFFER);
    sem_init(&llenas, 0, 0);

    pthread_t productor_id;
    pthread_t consumidor_id;

    pthread_create(&productor_id, NULL, &productor, NULL);
    pthread_create(&consumidor_id, NULL, &consumidor, NULL);
    
    pthread_join(productor_id, NULL);
    pthread_join(consumidor_id, NULL);

    return 0;
}

void* productor(void* unusued) {
    int elemento;	 
    while(1){
        elemento = producir_elemento();
        sem_wait(&mutex);//Se le envia el mutex para crear el interbolquoexcruir la memoria
        sem_wait(&vacias);        
        insertar_elemento(&elemento);
        sem_post(&mutex);
        sem_post(&llenas);
    }
}

int producir_elemento(void){
    int dato;    
    dato = rand() % 10;    
    return dato;   
}

void insertar_elemento(int *dato){
    buffer[posicion_in] = *dato;
    posicion_in = (posicion_in + 1) % MAX_BUFFER;
    contador++;
    printf("El productor produce %d en posicion %d\n", *dato, posicion_in); 
}

void* consumidor(void* unusued){
    int elemento;
    while(1){
        sem_wait(&llenas);
        sem_wait(&mutex);
        elemento = quitar_elemento();
        sem_post(&mutex);
        sem_post(&vacias);
        consumir_elemento(&elemento);
    }
}

int quitar_elemento(void){
    int dato;
    dato = buffer[posicion_out];
    buffer[posicion_out] = 0;
    posicion_out = (posicion_out + 1) % MAX_BUFFER;
    return dato;
}

void consumir_elemento(int *dato){
    int consumido = 1;
    printf("El consumidor consume %d producido en posición %d\n", *dato, posicion_out);
    *dato = consumido;
    contador--;
}
