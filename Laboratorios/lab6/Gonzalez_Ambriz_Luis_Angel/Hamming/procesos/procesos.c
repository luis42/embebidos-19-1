#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


#include "defs.h"
#include "procesamiento.h"
#include "datos.h"
extern float *A, *B, *C;



void proceso_hijo( int np, int pipefd[] ){

	register int i;

	//Calculando el inicio y fin de cada bloque
	int tamBloque = MUESTRAS / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque;

	printf("Proceso hijo %d con pid %d  \n\n", np, getpid());

	close( pipefd[0] );

	for( i = iniBloque; i < finBloque; i++){
		C[i] = A[i] * B[i];
	}

	write( pipefd[1], C+iniBloque, sizeof(int)*tamBloque );
	close(pipefd[1]);
	exit( np );
}

void proceso_padre( int pipefd[NUM_PROC][2] ){
	register int np;
        int pid, proc;

	int tamBloque = MUESTRAS / NUM_PROC;
	int iniBloque;

        printf("Proceso padre con pid %d\n", getpid());
        for( np = 0; np < NUM_PROC; np++){
		close( pipefd[np][1] );
		//Si no me importa el orden de los resultados no hay problema con un solo pipe
		//Pero si si es necesario entonces se deben crear una tuberia para cada proceso

                pid = wait( &proc );
		proc = proc >> 8;

		iniBloque = proc * tamBloque;
		read( pipefd[proc][0], C+iniBloque, sizeof(int)*tamBloque);
		printf("Proceso hijo %d, con pid %d\n\n", proc, pid);
		close( pipefd[proc][0] );

	}
}