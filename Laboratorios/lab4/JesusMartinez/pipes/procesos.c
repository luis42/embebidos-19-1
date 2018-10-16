#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "datos.h"
#include "procesamiento.h"
#include "defs.h"

void procesoPadre(int pipefd[])
{
		int np, npc, numero;
		int *orden = reservarMemoria();
		pid_t pid_hijo;
		
		close(pipefd[1]);
		for(np = 0; np < NUM_PROC; np++)
		{
			pid_hijo = wait(&npc);
			npc = npc >> 8;
			if(np == 3)
			{
				read(pipefd[0], orden, sizeof(int)*N);
			}
			else
			{
				read(pipefd[0], &numero, sizeof(int));
			}
			
			switch(npc)
			{
				case 0:
					printf("proceso hijo %i terminado con pid %i y mayor %i\n", npc, pid_hijo, numero);
				break;
				
				case 1:
					printf("proceso hijo %i terminado con pid %i y menor %i\n", npc, pid_hijo, numero);
				break;
				
				case 2:
					printf("proceso hijo %i terminado con pid %i y promedio %i\n", npc, pid_hijo, numero);
				break;
				
				case 3:
					imprimirArreglo(orden);
					printf("proceso hijo %i terminado con pid %i\n", npc, pid_hijo);
					free(orden);	
				break;
			}
		}
		//close(pipefd[0]);
}

void procesoHijo(int np, int *datos, int pipefd[])
{
	int mayor, menor, promedio;
	
	switch (np)
	{
		case 0:
			close(pipefd[0]);
			mayor = mayorArreglo(datos);
			write(pipefd[1], &mayor, sizeof(int));
			close(pipefd[1]);
			exit(np);
		break;
		
		case 1:
			close(pipefd[0]);
			menor = menorArreglo(datos);
			write(pipefd[1], &menor, sizeof(int));
			close(pipefd[1]);
			exit(np);
		break;
		
		case 2:
			close(pipefd[0]);
			promedio = promedioArreglo(datos);
			write(pipefd[1], &promedio, sizeof(int));
			close(pipefd[1]);
			exit(np);
		break;
		
		case 3:
			close(pipefd[0]);
			ordenarArreglo(datos);
			write(pipefd[1], datos, sizeof(int)*N);
			close(pipefd[1]);
			exit(np);
		break;
	}
}