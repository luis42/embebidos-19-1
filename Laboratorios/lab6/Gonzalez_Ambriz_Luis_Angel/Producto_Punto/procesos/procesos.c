#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


#include "defs.h"

extern int *A, *B;
extern int pp;

extern pthread_mutex_t bloqueo;


/* CON PROCESOS PERO ES MAS SENCILLO CON LA "Primera tecnica" pero lo haremos con la Segunda tecnica
*/

void proceso_hijo( int np, int pipefd[] ){

	register int i;

	printf("Proceso hijo %d con pid %d  \n\n", np, getpid());

	close( pipefd[0] );

	for( i = np; i < N; i+= NUM_PROC){
		pp += A[i] * B[i];
	}

	write( pipefd[1], &pp, sizeof(int) );
	close(pipefd[1]);
	exit( np );
}

void proceso_padre( int pipefd[] ){
	register int np;
        int pid, proc;
	int sumaParcial;

        printf("Proceso padre con pid %d\n", getpid());
	close( pipefd[1] );

	for( np = 0; np < NUM_PROC; np++){

                pid = wait( &proc );
		proc = proc >> 8;

		read( pipefd[0], &sumaParcial , sizeof(int) );
		pp += sumaParcial;

		printf("Proceso hijo %d terminado, con pid %d\n\n", proc, pid);

	}
	close( pipefd[0] );
}