#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "defs.h"



extern pthread_mutex_t bloqueo;
extern int *A, *B, pp;
void * funHilo( void *idh ){
	int nh = *(int *)idh;
	register int i;
	int sumaParcial = 0;

	int tamBloque = N / NUM_HILOS;
	int iniBloque = nh * tamBloque;
	int finBloque = iniBloque + tamBloque;

	for(i = iniBloque; i < finBloque; i++){
		sumaParcial += A[i] * B[i];
	}

	pthread_mutex_lock( &bloqueo );
	pp += sumaParcial;
	pthread_mutex_unlock( &bloqueo );

	pthread_exit( idh );
}
