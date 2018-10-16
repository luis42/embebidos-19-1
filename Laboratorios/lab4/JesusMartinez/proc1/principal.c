#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include"defs.h"
#include"datos.h"
#include"procesos.h"

int main()
{
	pid_t pid;
	int np;
	
	int *datos;
	
	datos = reservarMemoria();
	llenarArreglo(datos);
	
	printf("Los datos son:");
	imprimirArreglo(datos);
	printf("\n");

	printf("Probando procesos......\n");

	for(np = 0; np < NUM_PROC; np++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("No se creo el proceso....\n");
			exit(EXIT_FAILURE);
		}
		if(!pid)
		{
			procesoHijo(np, datos);
		}
	}	
	procesoPadre();
	return 0;
}
