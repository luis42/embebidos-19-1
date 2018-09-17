#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "datos.h"
#include "hilo.h"
#include "procesamiento.h"
#include "defs.h"


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
	guarda_datos(C, "sample.dat");

	free(A);
	free(B);
	free(C);

	return 0;
}

