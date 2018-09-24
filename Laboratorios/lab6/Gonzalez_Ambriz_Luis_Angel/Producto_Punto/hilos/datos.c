#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include "defs.h"



int * reservarMemoria( void ){
	int *mem;
	mem = (int *)malloc( sizeof(int)*N );
	if( !mem ){
		perror("Error al asignar memoria\n\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void imprimirArreglo( int datos[] ){
	register int n;
        for( n = 0; n < N; n++){
		if( !(n % 16))
			printf("\n");
        	printf("%3d ", datos[n]);
        }
	printf("\n\n");
}

void llenarArreglo( int datos[] ){
        register int n;
        for( n = 0; n < N; n++){
                datos[n] = rand() % 255;
        }
}
