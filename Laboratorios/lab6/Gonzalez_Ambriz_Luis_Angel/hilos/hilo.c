#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


#include "defs.h"
#include "procesamiento.h"
#include "datos.h"

void * funHilo( void *idh ){
	int nh = *(int *)idh;
	register int i;

	//Calculando el inicio y fin de cada bloque

	for(i = nh; i < MUESTRAS; i += NUM_HILOS){
		C[i] = A[i] * B[i];
	}

	pthread_exit( idh );
}