#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_HILOS 4
#define N 64

int *A , *B , *C;

int * reservarMemoria();
void llenarArreglo( int *datos );
void imprimirArreglo( int *datos );
void * funHilo( void *arg );


int main()
{
	srand( getpid() );

	int nhs[NUM_HILOS] , *res;
	register int nh;
	pthread_t tids[NUM_HILOS];

	printf("Probando hilos...\n");

	A = reservarMemoria();
	llenarArreglo( A );
	imprimirArreglo( A );

	B = reservarMemoria();
	llenarArreglo( B );
	imprimirArreglo( B );

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
	imprimirArreglo( C );
	
	free( A );
	free( B );
	free( C );
	
	return 0;
}

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

/*
void * funHilo( void *arg )
{
	register int i=0;
	int nh = *(int *)arg;
	int tamBloque = N / NUM_HILOS;
	int inicioBloque = nh * tamBloque;
	int finBloque = inicioBloque + tamBloque;

	for( i=inicioBloque ; i<finBloque ; i++ )
	{
		C[i] = A[i] * B[i];
	}
	pthread_exit( arg );
}
*/


void llenarArreglo( int *datos )
{
	register int i;

	//srand( getpid() );
	for( i=0 ; i<N ; i++ )
	{
		datos[i] = rand() % 255;
	}
}

int * reservarMemoria()
{
	int *mem;
	mem = (int *)malloc( sizeof(int) * N );

	if( !mem )
	{
		perror("Error al asginar memoria...\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void * menorArreglo( void *arg )
{
	register int i;
	static int menor;
	int *datos = (int *)arg;
	
	menor = datos[0];
	for( i=1 ; i<N ; i++)
	{
		if( datos[i] < menor )
		{
			menor = datos[i];
		}
	}
	pthread_exit(&menor);
}

void * mayorArreglo( void *arg )
{
	register int i;
	static int mayor;
	int *datos = (int *)arg;
	
	mayor = datos[0];
	for( i=1 ; i<N ; i++)
	{
		if( datos[i] > mayor )
		{
			mayor = datos[i];
		}
	}
	pthread_exit(&mayor);
}

void imprimirArreglo( int *datos )
{
	register int i;

	for( i=0 ; i<N ; i++)
	{
		if ( !(i%16) )
		{
			printf("\n");
		}
		printf("%5d ", datos[i]);
	}
	printf("\n");
}

