#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include "defs.h"

extern int *A , *B ;
extern int resultado;
extern pthread_mutex_t bloqueo;


void * funHilo( void *arg )
{
	register int i;
	int nh = *(int *)arg;
	int suma_parcial = 0;

	for( i=nh ; i<N ; i+=NUM_HILOS )
	{
		suma_parcial += A[i] * B[i];
	}

	pthread_mutex_lock( &bloqueo );
	resultado += suma_parcial;
	pthread_mutex_unlock( &bloqueo );

	pthread_exit( arg );
}
