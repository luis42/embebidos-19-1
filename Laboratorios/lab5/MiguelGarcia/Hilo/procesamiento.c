
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"

void * menorArreglo( void *arg )
{
	register int i ;
	int *datos = (int *)arg;
	int *menor = (int *)malloc(sizeof(int));

	pthread_mutex_lock( &bloqueo ); 

	*menor = datos[0];
	for( i=1 ; i<N ; i++)
	{
		if( datos[i] < *menor )
		{
			*menor = datos[i];
		}
	}

	pthread_mutex_unlock ( &bloqueo );
	pthread_exit( (void *)menor );
}

void * mayorArreglo( void *arg )
{
	register int i ;
	int *datos = (int *)arg;
	int *mayor = (int *)malloc(sizeof(int));

	pthread_mutex_lock( &bloqueo ); 
	
	*mayor = datos[0];
	for( i=1 ; i<N ; i++)
	{
		if( datos[i] > *mayor )
		{
			*mayor = datos[i];
		}
	}
	
	pthread_mutex_unlock ( &bloqueo );
	pthread_exit( (void *)mayor );
}

void * promedioArreglo( void *arg )
{
	register int i ;
	int *datos = (int *)arg;
	int *suma = (int *)malloc(sizeof(int));
	*suma = 0;

	pthread_mutex_lock( &bloqueo ); 

	for( i=0 ; i<N ; i++ )
	{
		*suma += datos[i];
	}
	*suma = *suma / N;

	pthread_mutex_unlock ( &bloqueo );
	pthread_exit( (void *)suma );
}

void ordenarArreglo( int *arg )
{
	register int i , j , temp;
	int *datos = (int *)arg;	

	pthread_mutex_lock( &bloqueo ); 	

	for( i=1 ; i<N ; i++ )
	{
		temp = datos[i];
		j = i-1;
		while( (datos[j]>temp) && (j>=0) )
		{
			datos[j+1] = datos[j];
			j--;
		}
		datos[j+1] = temp;
	}	

	pthread_mutex_unlock ( &bloqueo );
	pthread_exit( (void *)datos );
}

