#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#define NUM_HILOS 4
#define MUESTRAS 512

void * funHilo( void *idh );

float * reservarMemoria( void );
void imprimirArreglo( float datos[] );
void guarda_datos(float datos[], char *doc);

void llenar_seno(float seno[]);
void llenar_hamming(float hamming[]);

float *A, *B, *C;
float seno[MUESTRAS];
float hamming[MUESTRAS];

int main(){

	//Declaracion de variables
	register int nh;
	int  *hilo, nhs[NUM_HILOS];
	//Declaracion del hilo
	pthread_t tids[NUM_HILOS];

	//Reservar memoria
	A = reservarMemoria();
	B = reservarMemoria();
	C = reservarMemoria();

	//Llenar arreglos
	llenar_seno(A);
	llenar_hamming(B);

	//Imprimir los arreglos
	imprimirArreglo(A);
	guarda_datos(A, "seno.dat");
	imprimirArreglo(B);
	guarda_datos(B, "hamming.dat");

	//Creacion del hilo
	for(nh = 0; nh < NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create( &tids[nh], NULL, funHilo, (void *)&nhs[nh] );
	}

	//Esperar al hilo
	for(nh = 0; nh < NUM_HILOS; nh++){
		pthread_join( tids[nh], (void **)&hilo );
		printf("\n Terminando el hilo %d \n\n", *hilo);
	}

	imprimirArreglo(C);
	guarda_datos(C, "resultado.dat");

	free(A);
	free(B);
	free(C);

	return 0;
}

//SEGUNDA TECNICA

void * funHilo( void *idh ){
	int nh = *(int *)idh;
	register int i;

	//Calculando el inicio y fin de cada bloque

	for(i = nh; i < MUESTRAS; i += NUM_HILOS){
		C[i] = A[i] * B[i];
	}

	pthread_exit( idh );
}

float * reservarMemoria( void ){
	float *mem;
	mem = (float *)malloc( sizeof(float)*MUESTRAS );
	if( !mem ){
		perror("Error al asignar memoria\n\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void imprimirArreglo( float datos[] ){
	register int n;
        for( n = 0; n < MUESTRAS; n++){
		if( !(n % 16))
			printf("\n");
        	printf("%0.3f ", datos[n]);
        }
	printf("\n\n");
}

void llenar_seno(float seno[]){
	float f = 1000, fs = 45000;
	register int n;

	for(n = 0; n < MUESTRAS; n++){
		seno[n] = sinf(2*n*M_PI*f/fs);
	}
}

void llenar_hamming(float hamming[]){
	register int n;

	for(n = 0; n < MUESTRAS; n++){
		hamming[n] = 0.54-(0.46*cosf(2*n*M_PI/(MUESTRAS-1)));
	}
}

void guarda_datos(float datos[], char *doc){
	FILE *ap_arch;
	register int n;
	ap_arch = fopen(doc,"w");

	if(!ap_arch){
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	for(n = 0;n < MUESTRAS; n++){
		fprintf(ap_arch, "%f \n", datos[n]);
	}

	fclose(ap_arch);
}