#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "defs.h"
#include "datos.h"

extern int pp;
extern int *A;
extern int *B;

void procesoPadre(int pipefd[]){
	int np, npc, p;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d\n", getpid());
	
	close(pipefd[1]);

	for (np = 0; np < NUM_PROC; np++){
		
		pid_hijo = wait(&npc);
		npc = npc >> 8;

		read(pipefd[0], &p, sizeof(int));
		printf("Proceso hijo %d terminado con pid: %d \n", npc, pid_hijo);
		pp += p;
	}

	close(pipefd[0]);
	
}

void procesoHijo(int np, int pipefd[]){
	int suma_parcial = 0;
	register int i = 0;

	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid());

	close(pipefd[0]);

	for(i = np; i < N; i += NUM_PROC){
		suma_parcial = suma_parcial + A[i] * B[i];
	}

	write(pipefd[1], &suma_parcial, sizeof(int));
	close(pipefd[1]);
	exit(np);
}