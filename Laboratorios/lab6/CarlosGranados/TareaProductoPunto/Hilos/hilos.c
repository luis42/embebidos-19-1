#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "defs.h"

extern int pp, *A, *B;
extern pthread_mutex_t bloqueo;

void * funHilo(void *arg){
	int suma_parcial = 0;
	register int i = 0;
	int nh = *(int*) arg;

	for(i = nh; i <N; i += NUM_HILOS){
		suma_parcial = suma_parcial + A[i] * B[i];
	}

	pthread_mutex_lock(&bloqueo);
	pp += suma_parcial;
	pthread_mutex_unlock(&bloqueo);

	pthread_exit(arg);
}
