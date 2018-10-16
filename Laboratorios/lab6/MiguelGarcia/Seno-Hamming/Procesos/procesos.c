#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "defs.h"

extern float *A , *B , *C;


void procesoPadre( int pipefd[NUM_PROC][2] )
{
	int np , npc , tamBloque = N / NUM_PROC , iniBloque;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d \n" , getpid() );


	for( np = 0 ; np<NUM_PROC ; np++)
	{
		pid_hijo = wait( &npc );
		npc = npc >> 8;
		close( pipefd[npc][1] );
		iniBloque = npc * tamBloque;
		read( pipefd[npc][0] , C+iniBloque , sizeof(int)*tamBloque );
		printf("Proceso hijo %d terminado pid %d \n" , npc , pid_hijo );
		close( pipefd[npc][0] );
	}
}


void procesoHijo( int np , int pipefd[] )
{
	register int i;	

	int tamBloque = N / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque; 


	printf("Proceso hijo %d ejecutado con pid %d \n" , np , getpid() );
	close( pipefd[0] );

	for( i=iniBloque ; i<finBloque ; i++ )
	{
		C[i] = A[i] * B[i];
	}	

	write( pipefd[1] , C + iniBloque , sizeof(int)*tamBloque );
	close( pipefd[1] );

	exit( np );
}

