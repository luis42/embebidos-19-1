
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define N 32

int * reservarMemoria();

int main()
{
	int *datos;

	datos = reservarMemoria();
	llenarArreglo( datos );

	
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

