
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "defs.h"
#include "procesamiento.h"
#include "datos.h"

void procesoPadre( int pipefd[] )
{
	int np , npc , numero , datos[N];
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d \n" , getpid() );	
	close( pipefd[1] );
	
	for( np = 0 ; np < NUM_PROC ; np++ )
	{
		pid_hijo = wait( &npc );
		npc = npc >> 8;
		
		if( npc != 3)
		{
			read( pipefd[0] , &numero , sizeof(int) );
			printf("Proceso hijo %d terminado con pid %d y retorno %d \n", npc , pid_hijo , numero );
		}
		else
		{
			read( pipefd[0] , datos , sizeof(int)*N );
			printf("Proceso hijo %d terminado con pid %d y retorno del arreglo \n", npc , pid_hijo );
			imprimirArreglo( datos );
		}	
	}
	close( pipefd[0] );
	exit(0);
}

void procesoHijo( int np , int *datos , int pipefd[] )
{
	int mayor , menor , promedio;

	printf("Proceso hijo %d ejecutado con pid %d \n" , np , getpid() );	
	close( pipefd[0] );
	//while(1);
	
	if( np == 0 )
	{
		mayor = mayorArreglo( datos );
		write( pipefd[1] , &mayor , sizeof(int) );
		close( pipefd[1] );
		exit( np );
	}	
	else if( np == 1 )
	{
		menor = menorArreglo( datos );
		write( pipefd[1] , &menor , sizeof(int) );
		close( pipefd[1] );
		exit( np );
	}
	else if( np == 2 )
	{
		promedio = promedioArreglo( datos );
		write( pipefd[1] , &promedio , sizeof(int) );
		close( pipefd[1] );
		exit( np );
	}
	else if( np == 3 )
	{
		ordenarArreglo( datos );
		write( pipefd[1] , datos , sizeof(int)*N );
		close( pipefd[1] );	
		exit( np );
	}
}

