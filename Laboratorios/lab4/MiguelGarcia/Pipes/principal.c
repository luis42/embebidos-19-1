
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "procesos.h"
#include "datos.h"


int main()
{
	pid_t pid;
	int *datos , np;
	int pipefd[2] , pipe_status;
	
	datos = reservarMemoria();
	llenarArreglo( datos );
	imprimirArreglo( datos );

	printf("Probando procesos...");

	pipe_status = pipe(pipefd);

	if( pipe_status == -1)
	{
		perror("Error al crear tuberia");
		exit(EXIT_FAILURE);
	}

	for( np=0 ; np < NUM_PROC ; np++ )
	{
		pid = fork();

		if( pid == -1 )
		{
			perror("No se creo el proceso...\n");
			exit( EXIT_FAILURE );		
		}
		if( !pid )
		{
			procesoHijo( np , datos , pipefd );
		}
	}
	procesoPadre( pipefd );
	return 0;
}

