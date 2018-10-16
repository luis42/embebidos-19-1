#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "defs.h"
#include "datos.h"
#include "procesos.h"

int *A , *B ;
int resultado;

int main()
{
	printf("Producto Punto\n");
	
	pid_t pid;
	int np;
	int pipefd[NUM_PROC][2] , pipe_status;

	srand( getpid() );

	A = reservarMemoria();
	llenarArreglo( A );
	imprimirArreglo( A );

	B = reservarMemoria();
	llenarArreglo( B );
	imprimirArreglo( B );

	printf("Probando procesos...");
	resultado = 0;	
	
	for( np = 0 ; np < NUM_PROC ; np++ )
	{

		pipe_status = pipe( &pipefd[np][0] );
		if( pipe_status == -1)
		{
			perror("Error al crear la tuberia...\n");
			exit( EXIT_FAILURE );
		}

		pid = fork();
		if( pid == -1 )
		{
			perror("No se creo el proceso...\n");
			exit( EXIT_FAILURE );
		}
		if( !pid )
		{
			procesoHijo( np ,  &pipefd[np][0] );
		}

	}
	procesoPadre( pipefd );
	
	free( A );	
	free( B );	
	

	return 0;
}

