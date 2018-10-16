#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "datos.h"
#include "procesamiento.h"
#include "defs.h"

void procesoPadre()
{
		int np, numero;
		
		
		printf("Proceso padre ejecutando con pid %i\n", getpid());

		for(np = 0; np < NUM_PROC; np++)
		{
			pid_t pid_hijo;
			pid_hijo = wait(&numero);
			printf("proceso hijo terminado con pid %i y retorno %i\n", pid_hijo, numero>>8);
		}
}

void procesoHijo(int np, int *datos)
{
	int mayor, menor, promedio;
	
	switch (np)
	{
		case 0:
			mayor = mayorArreglo(datos);
			exit(mayor);
		break;
		
		case 1:
			menor = menorArreglo(datos);
			exit(menor);
		break;
		
		case 2:
			promedio = promedioArreglo(datos);
			exit(promedio);
		break;
		
		case 3:
			ordenarArreglo(datos);
			imprimirArreglo(datos);
			exit(np);
		break;
	}
}