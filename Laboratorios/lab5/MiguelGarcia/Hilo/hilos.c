
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "defs.h"
#include "procesamiento.h"
#include "datos.h"

void hilo( int *datos )
{
	int *res;
	pthread_t tids[NUM_HILOS];
	int nh;
	pthread_mutex_init( &bloqueo , NULL );

	for( nh=0 ; nh<NUM_HILOS ; nh++ )
	{
		if( nh==0 )
		{
			pthread_create( &tids[nh] , NULL , mayorArreglo , (void *)datos );
		}
		if( nh==1 )
		{
			pthread_create( &tids[nh] , NULL , menorArreglo , (void *)datos );
		}
		if( nh==2 )
		{
			pthread_create( &tids[nh] , NULL , promedioArreglo , (void *)datos );
		}
		if( nh==3 )
		{
			pthread_create( &tids[nh] , NULL , ordenarArreglo , (void *)datos );
		}
	}

	for( nh=0 ; nh<NUM_HILOS ; nh++ )
	{
		pthread_join( tids[nh] , (void **)&res );
		if( nh!=3 )
		{
			printf("Valor de retorno del hilo %d : %d \n" , nh , *res );
		}
		else
		{
			printf("Arreglo de retorno del hilo %d \n" , nh );
			imprimirArreglo( res );
		}
	}

	pthread_mutex_destroy( &bloqueo );
}
