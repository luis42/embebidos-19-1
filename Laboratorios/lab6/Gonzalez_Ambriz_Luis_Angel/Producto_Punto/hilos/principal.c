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

	register int nh;
	int  *hilo, nhs[NUM_HILOS];
	pthread_t tids[NUM_HILOS];

	A = reservarMemoria();
	B = reservarMemoria();
	llenarArreglo(A);
	
	llenarArreglo(B);

	imprimirArreglo(A);
	imprimirArreglo(B);

	pthread_mutex_init( &bloqueo, NULL );

	for(nh = 0; nh < NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create( &tids[nh], NULL, funHilo, (void *)&nhs[nh] );
	}

	for(nh = 0; nh < NUM_HILOS; nh++){
		pthread_join( tids[nh], (void **)&hilo );
		printf("\n Terminando el hilo %d \n\n", *hilo);
	}

	pthread_mutex_destroy( &bloqueo );

	printf("\n El producto punto es: %d \n\n", pp);
	free(A);
	free(B);
	return 0;
}