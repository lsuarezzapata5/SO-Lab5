#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#define BUFFER_SIZE 1024
#define NUMTHREADS 2

float contarNotas(FILE *file);
void leerArchivo(FILE *file, float *p);
void *calcularPromedio(void *apuntador);
void *calcularDesviacion(void *apuntador);

sem_t semaforo;
float desviacionEstandar=0;
float limite=0;
float promedio=0, suma=0;
float *notas,*p;



int main(int argc, char* argv[]){
	FILE *inFile;
	int i;
	float sumatoria=0;

	if (argc != 2){
		fprintf(stderr,"ejecute asi: a.out <nombreArchivo.csv > \n");
		return -1;
	}
	char *fileName = argv[1];
	inFile = fopen(fileName, "r");	
	
	if(inFile == NULL){
		printf("ERROR: No se puede abrir el archivo: %s\n", argv[1]);
		
		exit(0);
	}

	
	limite = contarNotas(inFile);

	printf("El Numero de notas:  %5.2f \n", limite);

	notas = (float *)malloc(sizeof(float)*limite);
	p=notas;	

	leerArchivo(inFile,p);
	
	for(i=0;i<=limite;i++){
		//printf("posicion %d: %f\n",i, *p);
		sumatoria+=*p;
		p++;		
	}
	printf("Sumatoria  %5.2f\n", sumatoria);
	
	pthread_t tid[NUMTHREADS];
	sem_init(&semaforo,0,0);
	
	pthread_create(&tid[0], NULL, calcularPromedio, (void*)&sumatoria);
	p=notas;
	pthread_create(&tid[1], NULL, calcularDesviacion, NULL);
	
	
	
	for(i=0; i <NUMTHREADS; i++){
		pthread_join(tid[i], NULL);
	}	

	
	sem_destroy(&semaforo);
	free(notas);
	fclose(inFile);


	printf("Promedio: %5.2f\n",promedio);
	printf("Desviacion Estandar: %5.2f\n",desviacionEstandar);
	
	return 0;
	
}

float contarNotas(FILE *file){
	int contador = 0;
	char buffer[BUFFER_SIZE];
	char *token;
	//while(fscanf(file,"%[^\n]s",buffer)!=feof(file)){
	while(fscanf(file,"%[^\n]s",buffer)!=feof(file)){
		token=strtok(buffer,";");

		while((token=strtok(NULL,";"))!=NULL){
			contador++;	
		}	
	}	
	rewind(file);
	return contador;
}

void leerArchivo(FILE *file, float *p){
	float contador = 0;
	float *apuntador=p;
	char buffer[BUFFER_SIZE];
	char *token;
	while(fscanf(file,"%[^\n]s",buffer)!=feof(file)){
		token=strtok(buffer,";");
		contador=(float)atof(token);
		*apuntador=contador;
		while((token=strtok(NULL,";"))!=NULL){
			apuntador++;
			contador=(float)atof(token);
			*apuntador=contador;	
		}	
	}	
	rewind(file);
}

void *calcularPromedio(void *param){
	float aux=*((float*)param);
	float prom=aux;
	promedio=prom/limite;
	sem_post(&semaforo);
	return 0;
}



void *calcularDesviacion(void *apuntador){
	sem_wait(&semaforo);
	float x=0;
	int i;
	float prom=promedio;
	//printf("Promedio:  %5.2f ", promedio);
	for(i=0;i<limite;i++){
		x+=pow((*p-prom),2);
		p++;
	}
	desviacionEstandar=sqrt(x/(limite-1));
	return 0;
}

