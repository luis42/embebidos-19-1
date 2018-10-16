
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROC 4
#define N 32

int * reservarMemoria();
void llenarArreglo(int *datos);
void imprimirArreglo(int *datos);
int mayorArreglo(int *datos);
int menorArreglo(int *datos);
float promedioArreglo(int *datos);
void ordenaArreglo(int **datos);

void procesoHijo(int np, int *datos);
void procesoPadre();

int main(){
	pid_t pid;
	int np;

	int *datos;

	datos = reservarMemoria();
	llenarArreglo(datos);
	imprimirArreglo(datos);

	printf("Probando procesos...");
	
	for (np = 0; np < NUM_PROC; np++)
	{
	
		pid = fork();
		if (pid== -1)
		{
			perror("No se creó el proceso... \n");
			exit(EXIT_FAILURE);
		}
		//Tiene pid = 0 es hijo, pid > 0 es padre
		if(!pid){
			procesoHijo(np, datos);
		}	
	}
	procesoPadre();	

	free(datos);

	return 0;
}



void procesoPadre()
{
	int np, numero;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d\n", getpid());
	
	for (np=0; np<NUM_PROC;np++)
	{
		pid_hijo=wait(&numero);
		printf("Proceso hijo terminado con pid: %d\n y retorno %d \n", pid_hijo, numero>>8);
	}
	
}

void procesoHijo(int np, int *datos)
{
	int mayor, menor;
	float promedio;

	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid());
	//while(1);

	if(np == 0){
		mayor = mayorArreglo(datos);
		exit(mayor);
	}
	else if(np == 1){
		menor = menorArreglo(datos);
		exit(menor);
	}
	else if(np == 2){
		promedio = promedioArreglo(datos);
		exit(promedio);
	}
	else if(np == 3){
		imprimirArreglo(datos); //sin ordenar
		ordenaArreglo(&datos);
		imprimirArreglo(datos); //ordenado
		exit(menor);
	}

}

void llenarArreglo(int *datos){
	register int i;

	srand(getpid()); //Semilla para generar random del número de proceso

	for(i = 0; i < N; i++){
		datos[i] = rand() % 255;
	}
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


void imprimirArreglo(int *datos){
	register int i;

	for(i = 0; i < N; i++){

		if(!(i%16))
			printf("\n");

		printf("%3d ", datos[i]);
	}
	printf("\n");
}

int mayorArreglo(int *datos){
	register int i , mayor;

	mayor = datos[0];

	for(i = 1; i < N; i++){
		if(datos[i]>mayor)
			mayor = datos[i];
	}

	return mayor;
}

int menorArreglo(int *datos){
	register int i , menor;

	menor = datos[0];

	for(i = 1; i < N; i++){
		if(datos[i]<menor)
			menor = datos[i];
	}

	return menor;
}

float promedioArreglo(int *datos){
	register int i, suma;
	float 

	for(i = 0; i < N; i++){
		suma = suma + datos[i];
	}

	return suma/N;
}

void ordenaArreglo(int **datos){
	register int i, j, imin, *aux;

	//Método de Selección
	for(i = 0; i < N-1; i++){
		imin = i;

		for (j = i+1; j > N; j++)
		{
			if(*datos[j] < *datos[imin]){
				imin = j;
			}
		}
		//intercambia i con imin
		aux = *datos[i];
		*datos[i] = *datos[imin];
		*datos[imin] = aux;

	}
}