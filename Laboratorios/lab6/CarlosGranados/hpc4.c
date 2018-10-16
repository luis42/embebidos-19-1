//Paralelismo para realizar el promedio
//Calcular el promedio del arreglo de forma concurrente con 4 hilos

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_HILOS 4
#define N 64 

int *A;
int promedio;

int * reservarMemoria();
void llenarArreglo(int *datos);
void imprimirArreglo(int *datos);
void * funHilo(void *arg);
pthread_mutex_t bloqueo;

int main(){
	int nhs[NUM_HILOS], *res;
	register int nh;
	pthread_t tids[NUM_HILOS];

	promedio = 0;

	printf("Probando hilos...\n");

	srand(getpid()); //Semilla para generar random del número de proceso

	pthread_mutex_init(&bloqueo, NULL);

	A = reservarMemoria();
	llenarArreglo(A);
	imprimirArreglo(A);

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

	printf("El promedio es: %d\n", promedio>>6); //corrimiento a la derecha es una división es una potencia de 2^N -> 2^6 = 64 por lo que equivale a promedio/64

	pthread_mutex_destroy(&bloqueo);
	free(A);

	return 0;
}


//Obtener la suma de las partes del arreglo correspondiente al hilo para regresarlo y finalmente obtener el promedio
void * funHilo(void *arg){
	register int i = 0;
	int nh = *(int*) arg;
	int suma_parcial = 0;

	//Funciona pero no es paralelo porque se detiene hasta que lo libere
	/*
	for(i = nh; i <N; i += NUM_HILOS){
		pthread_mutex_lock(&bloqueo);
		promedio += A[i];
		pthread_mutex_unlock(&bloqueo);
	}
	*/

	//De esta manera paralelizamos la suma parcial.
	for(i = nh; i < N; i += NUM_HILOS){
		suma_parcial += A[i];
	}
	//Si no queremos retornar suma_parcial mediante pthread_exit, se suma N veces bloqueando la variable promedio
	//Protegemos la recolección de datos porque es donde se guarda
	pthread_mutex_lock(&bloqueo);
	promedio += suma_parcial;
	pthread_mutex_unlock(&bloqueo);

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
