
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "hilos.h"
#include "datos.h"


int main()
{
	int *datos;
	
	datos = reservarMemoria();
	llenarArreglo( datos );
	imprimirArreglo( datos );

	printf("Probando hilos...\n");

	hilo( datos );
	
	return 0;
}

