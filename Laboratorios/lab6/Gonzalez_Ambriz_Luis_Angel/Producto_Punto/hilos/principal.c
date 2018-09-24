#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "datos.h"
#include "hilo.h"
#include "defs.h"
int *A, *B;
int pp = 0;

pthread_mutex_t bloqueo;

int main(){

	//Declaracion de variables
	register int nh;
	int  *hilo, nhs[NUM_HILOS];
	//Declaracion del hilo
	pthread_t tids[NUM_HILOS];

	//Reservar memoria
	A = reservarMemoria();
	B = reservarMemoria();
	//Llenar arreglos
	llenarArreglo(A);
	
	llenarArreglo(B);

	//Imprimir los arreglos
	imprimirArreglo(A);
	imprimirArreglo(B);

	//Creacion de mutex
	pthread_mutex_init( &bloqueo, NULL );

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

	//Destruir mutex
	pthread_mutex_destroy( &bloqueo );

	printf("\n El producto punto es: %d \n\n", pp);
	free(A);
	free(B);
	return 0;
}