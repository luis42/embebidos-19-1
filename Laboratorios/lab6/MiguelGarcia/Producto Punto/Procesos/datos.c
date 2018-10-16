#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include "defs.h"


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


void imprimirArreglo( int *datos )
{
	register int i;

	for( i=0 ; i<N ; i++)
	{
		if ( !(i%16) && i!=0 )
		{
			printf("\n");
		}
		printf("%d \t", datos[i]);
	}
	printf("\n");
}


