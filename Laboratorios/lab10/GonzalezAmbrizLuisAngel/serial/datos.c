#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "datos.h"

void imprimir(char *arreglo_dato)
{
	int inicio=strlen(arreglo_dato),i;
	
	for (i = 0; i < inicio; i++)
	{
		printf("%c",arreglo_dato[i]);
	}
	printf("\n Finaliza lectura de datos\n");

}



char * limpiar_cadena(char *arreglo_dato)
{
	register int i=0;
	for (i = 0; i < 200; i++)
	{
		arreglo_dato[i]='\0';
	}
	return arreglo_dato;

}

char * limpiar_c()
{
	register int i=0;
	char *arreglo_dato=(char*)malloc(60);
	for (i = 0; i < 60; i++)
	{
		arreglo_dato[i]='\0';
	}
	return arreglo_dato;

}



