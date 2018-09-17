#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "defs.h"

void guarda_datos(float datos[], char *doc){
	FILE *ap_arch;
	register int n;
	ap_arch = fopen(doc,"w");

	if(!ap_arch){
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	for(n = 0;n < MUESTRAS; n++){
		fprintf(ap_arch, "%f \n", datos[n]);
	}

	fclose(ap_arch);
}