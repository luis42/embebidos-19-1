#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROC 4
#define N 32

int *A;
int promedio;
int * reservarMemoria();
void llenarArreglo(int *datos);
void imprimirArreglo(int *datos);


void procesoHijo(int np, int pipefd[]);
void procesoPadre(int pipefd[NUM_PROC][2]);

int main(){
	pid_t pid;
	int np;

	//Este problema del promedio permite que se utilice un solo pipe pero se deja el arreglo por comodidad
	int *datos, pipefd[NUM_PROC][2], pipe_status;

	A = reservarMemoria();
	llenarArreglo(A);
	imprimirArreglo(A);

	printf("Probando procesos...\n");
	promedio = 0;

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
	
	free(A);
	
	return 0;
}


void procesoPadre(int pipefd[NUM_PROC][2])
{
	int np, npc, tamBloque=N/NUM_PROC, iniBloque, suma_parcial=0;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d\n", getpid());
	
	for (np=0; np<NUM_PROC;np++)
	{
		pid_hijo=wait(&npc);
		npc = npc>>8;
		close(pipefd[npc][1]);
		iniBloque = npc * tamBloque;
		read(pipefd[npc][0], &suma_parcial, sizeof(int));
		promedio += suma_parcial;
		printf("Proceso hijo %d terminado con pid: %d\n", npc, pid_hijo);
		close(pipefd[npc][0]);
	}

	printf("El promedio es: %d\n", promedio>>5);
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
		//En este caso no hay promedio con utilizar la variable así porque es con un bloque
		promedio += A[i];
	}
	write(pipefd[1], &promedio, sizeof(int));
	close(pipefd[1]);
	exit(np);
}

void llenarArreglo(int *datos){
	register int i;

	//srand(getpid()); //Semilla para generar random del número de proceso

	for(i = 0; i < N; i++){
		datos[i] = rand() % 255;
	}
}

int * reservarMemoria()
{
	int * mem;
	mem = (int *) malloc(sizeof(int)*N);
	if (!mem)
	{
		perror("Error al asignar memoria...\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void imprimirArreglo(int *datos){
	register int i;

	for(i = 0; i < N; i++){

		if(!(i%8))
			printf("\n");

		printf("%5d ", datos[i]);
	}
	printf("\n");
}

//TAREA para lunes
//Producto punto de dos arreglos con procesos e hilos