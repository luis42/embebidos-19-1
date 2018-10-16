#include <stdio.h>
#include <stdlib.h>
#include "archivos.h"
#include "procesamiento.h"
#include "defs.h"

int main(){
	float seno[MUESTRAS];

	genera_seno(seno);
	guarda_datos(seno);

	FILE *pipe_gp = popen("gnuplot -p", "w");
  	fputs("plot \"seno.dat\" w l \n", pipe_gp);
	pclose(pipe_gp);

	return 0;
}
