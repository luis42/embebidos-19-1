//Programa que multiplica dos arreglos: uno con valores de Seno y otro con valores de la ventana de Hamming. El resultado de la multiplicación se almacena en otro arreglo. 
//Se utilizan procesos para calcular segmentos de los arreglos y así obtener el arreglo con el resultado de la multiplicación.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
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

void procesoHijo(int np, int pipefd[]);
void procesoPadre(int pipefd[NUM_PROC][2]);

int main(){
	pid_t pid;
	int np;

	int pipefd[NUM_PROC][2], pipe_status;

	A = reservarMemoria();
	llenarArregloSeno(A);
	imprimirArreglo(A);
	guarda_datos(0, A);

	B = reservarMemoria();
	llenarArregloHamming(B);
	imprimirArreglo(B);
	guarda_datos(1, B);

	C = reservarMemoria();

	printf("\nProbando procesos...\n");


	for (np = 0; np < NUM_PROC; np++)
	{
		pipe_status = pipe(&pipefd[np][0]);
		
		if (pipe_status == -1)
		{
			perror("Error al crear la tubería...\n");
			exit(EXIT_FAILURE);
		}

		pid = fork();
		if (pid== -1)
		{
			perror("No se creó el proceso... \n");
			exit(EXIT_FAILURE);
		}
		//Tiene pid = 0 es hijo, pid > 0 es padre
		if(!pid){
			procesoHijo(np, &pipefd[np][0]);
		}
	}

	procesoPadre(pipefd);
	imprimirArreglo(C);
	guarda_datos(2, C);

	free(A);
	free(B);
	free(C);

	graficar();

	return 0;
}


void procesoPadre(int pipefd[NUM_PROC][2])
{
	int np, npc, tamBloque=N/NUM_PROC, iniBloque;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d\n", getpid());
	
	for (np=0; np<NUM_PROC;np++)
	{
		pid_hijo=wait(&npc);
		npc = npc>>8;
		close(pipefd[npc][1]);
		iniBloque = npc * tamBloque;
		read(pipefd[npc][0], C+iniBloque, sizeof(float)*tamBloque);
		printf("Proceso hijo %d terminado con pid: %d\n", npc, pid_hijo);
		close(pipefd[npc][0]);
	}
}

void procesoHijo(int np, int pipefd[])
{
	register int i;
	int tamBloque = N/NUM_PROC;
	int iniBloque = np * tamBloque; 
	int finBloque = iniBloque + tamBloque;
	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid());

	close(pipefd[0]);
	for (i = iniBloque; i<finBloque; i++)
	{
		C[i] = A[i] * B[i];
	}
	write(pipefd[1], C+iniBloque, sizeof(float)*tamBloque);
	close(pipefd[1]);
	exit(np);
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


/*
-> principal.c

int *A, *B, *C
int main(){
	
	return 0;
}

-> hilos.c

extern int *A, *B, *C
funhilo()
...

*/