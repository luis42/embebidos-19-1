#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "defs.h"
#include "procesamiento.h"
#include "datos.h"

void procesoPadre(int pipefd[]){
	int np, npc, numero, numOrdena[N];
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d\n", getpid());
	
	close(pipefd[1]);

	for (np = 0; np < NUM_PROC; np++){
		
		pid_hijo = wait(&npc);
		npc = npc >> 8;

		switch(npc){
			case 0:
				read(pipefd[0], &numero, sizeof(int));
				printf("Proceso hijo %d terminado con pid: %d y mayor %d \n", npc, pid_hijo, numero);
				break;

			case 1:
				read(pipefd[0], &numero, sizeof(int));
				printf("Proceso hijo %d terminado con pid: %d y menor %d \n", npc, pid_hijo, numero);
				break;

			case 2:
				read(pipefd[0], &numero, sizeof(int));
				printf("Proceso hijo %d terminado con pid: %d y promedio %d \n", npc, pid_hijo, numero);
				break;

			case 3:
				read(pipefd[0], numOrdena, sizeof(int)*N);
				printf("Proceso hijo %d terminado con pid: %d\n", npc, pid_hijo);
				imprimirArreglo(numOrdena);
				break;

			default:
				printf("Algo salió mal...");
		}

	}

	close(pipefd[0]);
	
}

void procesoHijo(int np, int *datos, int pipefd[]){
	int mayor, menor;
	int prom;

	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid());
	//while(1);

	close(pipefd[0]);

	switch(np){
		case 0:
			mayor = mayorArreglo(datos);
			write(pipefd[1], &mayor, sizeof(int));
			close(pipefd[1]);

			exit(np);

			break;

		case 1:
			menor = menorArreglo(datos);
			write(pipefd[1], &menor, sizeof(int));
			close(pipefd[1]);

			exit(np);

			break;

		case 2:
			prom = promedioArreglo(datos);
			write(pipefd[1], &prom, sizeof(int));
			close(pipefd[1]);

			exit(np);

			break;

		case 3:
			ordenaArreglo(datos);
			write(pipefd[1], datos, sizeof(int)*N);
			close(pipefd[1]);

			exit(np);

			break;

		default:
			printf("Algo salió mal...");
	}

}