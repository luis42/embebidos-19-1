#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include"defs.h"
#include"datos.h"
#include"procesamiento.h"
#include"procesos.h"

int main()
{
	pid_t pid;
	int np;
	
	int *datos, pipefd[2], pipe_status;	
	
	datos = reservarMemoria();
	llenarArreglo(datos);
	printf("Los datos son:\n");
	imprimirArreglo(datos);

	printf("\nProbando procesos......\n");
	
	pipe_status = pipe(pipefd);
	
	if(pipe_status != 0)
	{
		printf("No pudo crearse la tuberia\n");
		exit(-1);
	}

	for(np = 0; np < NUM_PROC; np++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("No se creo el proceso....\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0)
		{
			procesoHijo(np, datos, pipefd);
		}
	}	
	procesoPadre(pipefd);
	return 0;
}