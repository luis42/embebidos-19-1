#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 32

int * reservarMemoria();
void llenarArreglo( int *datos );
void imprimirArreglo( int *datos );
void * mayorArreglo( void *arg );
void * menorArreglo( void *arg );

//int num1=52 , num2 = 10;

int main()
{
	int *res_may , *res_men;
	pthread_t tid_may , tid_men;

	int *datos;

	printf("Probando hilos...\n");

	datos = reservarMemoria();
	llenarArreglo( datos );
	imprimirArreglo( datos );
	
	pthread_create( &tid_men , NULL , menorArreglo , (void *)datos );
	pthread_create( &tid_may , NULL , mayorArreglo , (void *)datos );
	

	pthread_join( tid_men , (void **)&res_men );
	pthread_join( tid_may , (void **)&res_may );
	

	printf("El mayor es: %d\n" , *res_may );
	printf("El menor es: %d\n" , *res_men );
	
	return 0;
}


void llenarArreglo( int *datos )
{
	register int i;

	srand( getpid() );
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
		printf("%3d ", datos[i]);
	}
	printf("\n");
}
