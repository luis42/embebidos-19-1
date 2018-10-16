#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include "defs.h"

extern float *A , *B , *C;


void * funHilo( void *arg )
{
	register int i=0;
	int nh = *(int *)arg;

	for( i=nh ; i<N ; i+=NUM_HILOS )
	{
		C[i] = A[i] * B[i];
	}
	pthread_exit( arg );
}
