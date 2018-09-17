#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include "defs.h"

extern float seno[MUESTRAS];
extern float hamming[MUESTRAS];


float * reservarMemoria( void ){
	float *mem;
	mem = (float *)malloc( sizeof(float)*MUESTRAS );
	if( !mem ){
		perror("Error al asignar memoria\n\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}




void imprimirArreglo( float datos[] ){
	register int n;
        for( n = 0; n < MUESTRAS; n++){
		if( !(n % 16))
			printf("\n");
        	printf("%0.3f ", datos[n]);
        }
	printf("\n\n");
}



void llenar_seno(float seno[]){
	float f = 1000, fs = 45000;
	register int n;

	for(n = 0; n < MUESTRAS; n++){
		seno[n] = sinf(2*n*M_PI*f/fs);
	}
}

void llenar_hamming(float hamming[]){
	register int n;

	for(n = 0; n < MUESTRAS; n++){
		hamming[n] = 0.53836-(0.46164*cosf(2*n*M_PI/(MUESTRAS-1)));
	}
}

