#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

sem_t cliente;
sem_t permisoDeProcesamiento;

int sillasLibres = 5;

void* funcionBarbero();
void* funcionCliente();

int main(void)
{
	printf("\t\tEL BARBERO DORMILON.\n\n");
	
    sem_init(&cliente, 0, 0); // Semaforo de sincronizacion para el numero de clientes.
    sem_init(&permisoDeProcesamiento, 0, 1); //Semaforo binario empleado para controlar el acceso a la zona crítica.
    
    //Declaracion de los hilos
	pthread_t hiloBarbero;
	pthread_t hiloClientes;
	
	//Creacion de hilos.
	pthread_create(&hiloBarbero, NULL, &funcionBarbero, NULL);	
	pthread_create(&hiloClientes, NULL, &funcionCliente, NULL);
		
	//Join de hilos	
	pthread_join(hiloBarbero, NULL);	
	pthread_join(hiloClientes, NULL);
	return 0;
}

void* funcionCliente()
{
	while(1)
	{
		sem_wait(&permisoDeProcesamiento); // Ingreso exclusivamente a la zona critica del programa. La bloqueo.
		
		printf("El cliente llego a la barberia. Sillas disponibles %d.\n", sillasLibres);
		if(sillasLibres > 0) //Verifico si hay sillas disponibles.
		{			
			sillasLibres--; // Ocupo una silla.
			sem_post(&cliente);// Me pongo en cola.
			printf("El cliente esta esperando a ser atendido. Sillas disponibles %d.\n", sillasLibres);
			sem_post(&permisoDeProcesamiento); // Libero el acceso a la zona critica.
			
			sleep(2);
		}
		else
		{
			sem_post(&permisoDeProcesamiento); // Libero el acceso a la zona critica.
			printf("No tenemos mas sillas. El cliente se ha ido de la barberia.\n");
			sleep(1);
		}
	}
}

void* funcionBarbero()
{
	while(1) //Inicio el cilco de trabajo.
	{
		sem_wait(&cliente); // Tomo un cliente de la lista de espera, si no hay me acuesto a dormir.
		sem_wait(&permisoDeProcesamiento); // Ingreso exclusivamente a la zona critica del programa. La bloqueo.
		sillasLibres ++; // Libero una silla.
		sem_post(&permisoDeProcesamiento);	//Libero el acceso a la zona critica.
			
		printf("\nBarbero: Cortando ...\n"); // El barbero ejecuta su trabajo.
		sleep(5);
		printf("Corte terminado.\n"); // El barbero ejecuta su trabajo.
	}
}
