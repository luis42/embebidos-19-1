
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
	
	datos = reservarMemoria();
	llenarArreglo( datos );
	imprimirArreglo( datos );

	printf("Probando procesos...");

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
			procesoHijo( np , datos );
		}
	}
	procesoPadre();
	return 0;
}

