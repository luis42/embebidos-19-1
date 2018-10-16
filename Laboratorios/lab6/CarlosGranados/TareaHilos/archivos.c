#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

void guarda_datos(int opc, float datos[]){
	FILE *ap_arch;
	register int n;

	switch(opc){
		case 0:
			ap_arch = fopen("seno.dat", "w");
			break;

		case 1:
			ap_arch = fopen("hamming.dat", "w");
			break;

		case 2:
			ap_arch = fopen("resultado.dat", "w");
			break;

		default:
			printf("Opción inválida :(\n");
	}
	
	if (!ap_arch)
	{
		perror("Error al abrir el archivo.");
		exit(EXIT_FAILURE);
	}

	for (n = 0; n < N; n++)
	{
		fprintf(ap_arch, "%f \n", datos[n]);
	}

	fclose(ap_arch);
}