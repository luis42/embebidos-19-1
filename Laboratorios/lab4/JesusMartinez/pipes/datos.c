#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "defs.h"

int * reservarMemoria()
{
	int * mem;
	mem = (int *)malloc(sizeof(int)*N);
	
	if(!mem)
	{
		perror("Error al asignar memoria...\n");
		exit(EXIT_FAILURE);
	}
	
	return mem;
}

void llenarArreglo(int * datos)
{
	register int i;
	
	srand( getpid() );
	for(i=0; i<N; i++)
	{
		datos[i] = rand() % 255;
	}
}

void imprimirArreglo(int *datos)
{
	register int i;
	
	for(i=0; i<N; i++)
	{
		if((i%16) == 0 && i!=0)
			printf("\n");
		printf("%3d ", datos[i]);
	}
	printf("\n");
}