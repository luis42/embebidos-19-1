
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "archivos.h"
#include "defs.h"

float *A, *B, *C;
float * reservarMemoria();
void llenarArregloSeno(float *datos);
void llenarArregloHamming(float *datos);
void imprimirArreglo(float *datos);
void graficar();

void * funHilo(void *arg);
pthread_mutex_t bloqueo;


int main(){
	int nhs[NUM_HILOS], *res;
	register int nh;

	pthread_t tids[NUM_HILOS];

	A = reservarMemoria();
	llenarArregloSeno(A);
	imprimirArreglo(A);
	guarda_datos(0, A);

	B = reservarMemoria();
	llenarArregloHamming(B);
	imprimirArreglo(B);
	guarda_datos(1, B);

	C = reservarMemoria();

	//Se agregó para tener exclusión mutua; Se inicializa en el proceso padre una vez
	pthread_mutex_init(&bloqueo, NULL);
	
	printf("Probando hilos...\n");

	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, funHilo, (void *)&nhs[nh]);
	}
	
	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **)&res);
		printf("Hilo %d terminado\n", *res);
	}

	pthread_mutex_destroy(&bloqueo);

	imprimirArreglo(C);
	guarda_datos(2, C);

	free(A);
	free(B);
	free(C);

	graficar();

	return 0;
}


void * funHilo(void *arg){
	int nh = *(int*) arg;

	register int i = 0;
	int tamBloque = N/NUM_HILOS;
	int iniBloque = nh * tamBloque; 
	int finBloque = iniBloque + tamBloque;


	//Se agregó esto para crear una zona crítica y que los demás hilos no puedan acceder a ella mientras uno la esté ocupando
	pthread_mutex_lock(&bloqueo);

	printf("Iniciando hilo %d\n", nh);

	for (i = iniBloque; i<finBloque; i++)
	{
		C[i] = A[i] * B[i];
	}

	printf("Terminando hilo %d\n", nh);


	pthread_mutex_unlock(&bloqueo);

	pthread_exit(arg);
}



float * reservarMemoria()
{
	float * mem;
	mem = (float *) malloc(sizeof(float)*N);
	if (!mem)
	{
		perror("Error al asignar memoria...\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void llenarArregloSeno(float *datos){
	register int i;
	float f = 1000, fs = 45000;

	for (i = 0; i < N; i++)
	{
		datos[i] = sinf(2 * i * M_PI * f / fs);
	}
}

void llenarArregloHamming(float *datos){
	register int i;

	for(i = 0; i < N; i++){
		datos[i] = 0.54 - (0.46 * cosf(2 * M_PI * i / N));
	}
}

void imprimirArreglo(float *datos){
	register int i;

	for(i = 0; i < N; i++){

		if(!(i%8))
			printf("\n");

		printf("%5.3f ", datos[i]);
	}
	printf("\n");
}

void graficar(){
	FILE *pipe_gp = popen("gnuplot -p", "w");

	fputs("set multiplot \n", pipe_gp);
	fputs("set size .5,.5 \n", pipe_gp);

	fputs("set origin 0,.5 \n", pipe_gp);
  	fputs("plot \"seno.dat\" w l \n", pipe_gp);

  	fputs("set origin 0,0 \n", pipe_gp);
  	fputs("plot \"hamming.dat\" w l \n", pipe_gp);

  	fputs("set origin .5,.25 \n", pipe_gp);
  	fputs("plot \"resultado.dat\" w l \n", pipe_gp);

	pclose(pipe_gp);
}

