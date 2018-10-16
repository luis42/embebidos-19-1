//Calculo de producto punto de dos arreglos utilizando hilos

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "datos.h"
#include "hilos.h"
#include "defs.h"

pthread_mutex_t bloqueo;
int np, *A, *B, pp = 0;

int main(){
	int nhs[NUM_HILOS], *res;
	register int nh;
	pthread_t tids[NUM_HILOS];
	
	//srand(getpid());

	A = reservarMemoria();
	llenarArreglo(A);
	imprimirArreglo(A);

	B = reservarMemoria();
	llenarArreglo(B);
	imprimirArreglo(B);

	pthread_mutex_init(&bloqueo, NULL);

	printf("\nProbando hilos...\n");
	
	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, funHilo, (void *)&nhs[nh]);
	}
	
	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **)&res);
		printf("Hilo %d terminado\n", *res);
	}

	pthread_mutex_destroy(&bloqueo);

	printf("\nEl producto punto es: %d \n", pp);

	free(A);
	free(B);

	return 0;
}