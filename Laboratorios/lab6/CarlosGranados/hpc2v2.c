//Paralelismo a nivel de datos dividiendo inicio/fin de bloque
//Tiene un problema cuando la división de bloques no es exacta por el número de cores que se tienen

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_HILOS 4
#define N 64 

int *A, *B, *C;

int * reservarMemoria();
void llenarArreglo(int *datos);
void imprimirArreglo(int *datos);
void * funHilo(void *arg);

int main(){
	int nhs[NUM_HILOS], *res;
	register int nh;
	pthread_t tids[NUM_HILOS];

	printf("Probando hilos...\n");

	srand(getpid()); //Semilla para generar random del número de proceso

	A = reservarMemoria();
	llenarArreglo(A);
	imprimirArreglo(A);

	B = reservarMemoria();
	llenarArreglo(B);
	imprimirArreglo(B);

	C = reservarMemoria();


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

	imprimirArreglo(C);

	free(A);
	free(B);
	free(C);

	return 0;
}


void * funHilo(void *arg){
	register int i = 0;
	int nh = *(int*) arg;

	for(i = nh; i <N; i += NUM_HILOS){
		C[i] = A[i] * B[i];
	}

	pthread_exit(arg);
}


int * reservarMemoria(){
	int *mem;

	mem = (int *) malloc(sizeof(int)*N);
	if(!mem){
		perror("Error al asignar memoria...\n");
		exit(EXIT_FAILURE);
	}

	return mem;
}


void llenarArreglo(int *datos){
	register int i;

	for(i = 0; i < N; i++){
		datos[i] = rand() % 255;
	}
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
