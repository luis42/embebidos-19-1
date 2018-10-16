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
void producto( void );


int main()
{
	srand( getpid() );	

	A = reservarMemoria();
	llenarArreglo( A );
	imprimirArreglo( A );

	B = reservarMemoria();
	llenarArreglo( B );
	imprimirArreglo( B );

	C = reservarMemoria();
	producto();
	imprimirArreglo( C );
	
	free( A );
	free( B );
	free( C );

	return 0;
}


void llenarArreglo( int *datos )
{
	register int i;

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

void producto( void )
{
	register int i;

	for( i=0 ; i<N ; i++ )
	{
		C[i] = A[i] * B[i];
	}
}


