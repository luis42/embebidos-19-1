#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include "defs.h"
#include "datos.h"
#include "hilos.h"

int *A , *B ;
int resultado;
pthread_mutex_t bloqueo;

int main()
{
	printf("Producto Punto\n");
	
	int nhs[NUM_HILOS] , *res;
	register int nh;
	pthread_t tids[NUM_HILOS];

	srand( getpid() );

	A = reservarMemoria();
	llenarArreglo( A );
	imprimirArreglo( A );

	B = reservarMemoria();
	llenarArreglo( B );
	imprimirArreglo( B );

	printf("Probando hilos...");
	resultado = 0;	
	
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

	printf("Resultado: %d \n" , resultado );

	//imprimirArreglo( A );
	
	free( A );
	free( B );
	pthread_mutex_destroy( &bloqueo );
	
	return 0;
}

