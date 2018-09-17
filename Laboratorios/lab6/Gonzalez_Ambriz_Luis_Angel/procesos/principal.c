#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "datos.h"
#include "procesos.h"
#include "procesamiento.h"
#include "defs.h"


int main(){

	//Declaracion de variables
	pid_t pid;
	int pipefd[NUM_PROC][2], edo_pipe;
	register int np;

	//Reservar memoria
	A = reservarMemoria();
	B = reservarMemoria();
	C = reservarMemoria();

	//Llenar arreglos
	llenar_seno(A);
	llenar_hamming(B);

	//Imprimir los arreglos
	imprimirArreglo(A);
	guarda_datos(A, "seno.dat");
	imprimirArreglo(B);
	guarda_datos(B, "hamming.dat");

	for( np = 0; np < NUM_PROC; np++){
		edo_pipe = pipe(&pipefd[np][0]);

		if( edo_pipe == -1 ){
			perror("Error al crear el pipe\n");
			exit(EXIT_FAILURE);
		}

                pid = fork();

                if( pid == -1){
                        perror("Error al crear el proceso hijo\n");
                        exit(EXIT_FAILURE);
                }

                if( !pid ){
                        proceso_hijo( np, &pipefd[np][0] );
                }
        }

        proceso_padre( pipefd );

	imprimirArreglo(C);
	guarda_datos(C, "sample.dat");
	free(A);
	free(B);
	free(C);

	return 0;
}

