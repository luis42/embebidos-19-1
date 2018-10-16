//Calculo de producto punto de dos arreglos

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "datos.h"
#include "procesos.h"
#include "defs.h"

int np, *A, *B, pipefd[2], pipe_status, pp;

int main(){
	pid_t pid;

	//srand(getpid());

	A = reservarMemoria();
	llenarArreglo(A);
	imprimirArreglo(A);

	B = reservarMemoria();
	llenarArreglo(B);
	imprimirArreglo(B);

	printf("\nProbando procesos...\n");
	pipe_status = pipe(pipefd);

	if (pipe_status == -1){
		perror("Error al crear la tubería...n");
		exit(EXIT_FAILURE);
	}
	
	for (np = 0; np < NUM_PROC; np++){
	
		pid = fork();
		if (pid == -1){
			perror("No se creó el proceso... \n");
			exit(EXIT_FAILURE);
		}
		//Tiene pid = 0 es hijo, pid > 0 es padre
		if(!pid){
			procesoHijo(np, pipefd);
		}	
	}
	procesoPadre(pipefd);

	printf("\nEl producto punto es: %d \n", pp);

	free(A);
	free(B);

	return 0;
}
