#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "datos.h"
#include "defs.h"
#include "procesos.h"


int *A, *B;
int pp;

int main(){

	//Declaracion de variables
	pid_t pid;
	int pipefd[2], edo_pipe;
	register int np;

	//Reservar memoria
	A = reservarMemoria();
	B = reservarMemoria();
	//Llenar arreglos
	llenarArreglo(A);
	
	llenarArreglo(B);
	

	//Imprimir los arreglos
	imprimirArreglo(A);
	imprimirArreglo(B);

	edo_pipe = pipe( pipefd);

	if( edo_pipe == -1 ){
		perror("Error al crear el pipe\n");
		exit(EXIT_FAILURE);
	}


	for( np = 0; np < NUM_PROC; np++){
                pid = fork();
                if( pid == -1){
                        perror("Error al crear el proceso hijo\n");
                        exit(EXIT_FAILURE);
                }
                if( !pid ){
                        proceso_hijo( np, pipefd );
                }
        }

        proceso_padre( pipefd );

	printf("\n El producto punto es: %d \n\n", pp);
	
	free(A);
	free(B);

	return 0;
}

