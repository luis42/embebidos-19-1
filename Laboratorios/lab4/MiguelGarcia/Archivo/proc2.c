
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define NUM_PROC 4

void procesoHijo( int np );
void procesoPadre();

int main()
{
	pid_t pid;
	int np;

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
			procesoHijo( np );
		}
	}
	procesoPadre();
	return 0;
}


void procesoPadre()
{
	int np , estado;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d \n" , getpid() );	
	
	for( np = 0 ; np < NUM_PROC ; np++ )
	{
		pid_hijo = wait( &estado );
		printf("Proceso hijo %d terminado con pid %d \n", estado>>8 , pid_hijo );	
	}
}

void procesoHijo( int np )
{
	printf("Proceso hijo %d ejecutado con pid %d \n" , np , getpid() );	
	while(1);
	exit(np);
}
