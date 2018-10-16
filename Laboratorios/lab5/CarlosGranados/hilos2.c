
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 32

int * reservarMemoria();
void llenarArreglo(int *datos);
void imprimirArreglo(int *datos);
void * mayorArreglo(void *datos);
void * menorArreglo(void *datos);

//Se ponen globales para que sean compartidos entre los 4 hilos
int num1 = 52, num2 = 10;

int main(){
	int *res_may, *res_men, *datos;

	pthread_t tid_may, tid_men;

	//
	printf("Probando hilos...\n");

	datos = reservarMemoria();
	llenarArreglo(datos);
	imprimirArreglo(datos);

	pthread_create(&tid_men, NULL, menorArreglo, datos); //Se pasa por parámetro
	pthread_create(&tid_may, NULL, mayorArreglo, datos);

	//Pide un doble apuntador por eso se utiliza & con la variable *
	//Y hacer el cast porque pide un void**
	pthread_join(tid_men, (void **) &res_men);
	pthread_join(tid_may, (void **) &res_may);

	printf("El número menor es: %d \n", *res_men);
	printf("El número mayor es: %d \n", *res_may);

	return 0;
}


void * mayorArreglo(void *arg){
	register int i;
	static int mayor;
	int *datos = (int*) arg;

	mayor = datos[0];

	for(i = 1; i < N; i++){
		if(datos[i]>mayor)
			mayor = datos[i];
	}

	pthread_exit((void*) &mayor);
}

void * menorArreglo(void *arg){
	register int i;
	static int menor;
	int *datos = (int*) arg;

	menor = datos[0];

	for(i = 1; i < N; i++){
		if(datos[i]<menor)
			menor = datos[i];
	}

	pthread_exit((void *) &menor);
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

	srand(getpid()); //Semilla para generar random del número de proceso

	for(i = 0; i < N; i++){
		datos[i] = rand() % 255;
	}
}


void imprimirArreglo(int *datos){
	register int i;

	for(i = 0; i < N; i++){

		if(!(i%16))
			printf("\n");

		printf("%3d ", datos[i]);
	}
	printf("\n");
}