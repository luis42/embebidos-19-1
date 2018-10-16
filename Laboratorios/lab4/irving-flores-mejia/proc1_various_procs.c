#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define NUM_PROC 4


void procesoPadre()
{
		int np, estado;
		pid_t pid_hijo;
		printf("Proceso padre ejecutado: %d\n", getpid());
		
		for ( np = 0; np < NUM_PROC ; np++)
		{
			pid_hijo = wait(&estado);
			printf("Proceso hijo %d terminado con pid %d \n", estado>>8 ,pid_hijo);
		}
}

void procesoHijo( int np)
{
	printf("Proceso hijo %d ejecutando: %d \n", np ,getpid()); 
	//no hacer esto
	while(1);
	exit(np);//se sabe que hijo ha terminado
}

int main(){
	pid_t pid;
	int np;
	
	printf("Probando procesos \n");
	
	for (np = 0; np < NUM_PROC; np++)
	{
		pid = fork();
		if ( pid == -1)
			{
				perror("No se creo el proceso :( \n"); 
				exit(EXIT_FAILURE);
			}
		if(!pid)
		{
	     	 procesoHijo( np );
		}
	}
		//sleep(5);
		procesoPadre();  
	return 0;
}
