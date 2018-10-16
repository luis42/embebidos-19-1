#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include "defs.h"
#include "datos.h"
#include "hilos.h"

float *A , *B , *C;

int main()
{
	srand( getpid() );

	int nhs[NUM_HILOS] , *res;
	register int nh;
	pthread_t tids[NUM_HILOS];

	printf("Probando hilos...\n");

	A = reservarMemoria();
	llenarSeno( A );
	//imprimirArreglo( A );

	B = reservarMemoria();
	llenarHamming( B );
	//imprimirArreglo( B );

	C = reservarMemoria();
	
	for( nh = 0 ; nh < NUM_HILOS ; nh++ )
	{
		nhs[nh] = nh;
		pthread_create( &tids[nh] , NULL , funHilo , (void *)&nhs[nh] );
	}

	for( nh = 0 ; nh < NUM_HILOS ; nh++ )
	{
		pthread_join( tids[nh] , (void **)&res );
		printf("Hilo %d terminado \n" , *res );
	}
	guardaArreglo( C );
	
	free( A );
	free( B );
	free( C );
	
	return 0;
}

