#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROC 4
#define N 32 //Con números grandes hay problemas con la sinconía pero 

//programa para crear muchos hilos
int *A;
int promedio;
int * reservarMemoria();
void llenarArreglo(int *datos);
void imprimirArreglo(int *datos);

void proceso_hijo( int np, int pipefd[] );
void proceso_padre( int pipefd[NUM_PROC][2]);
//void * funHilo(void *arg);
//pthread_mutex_t bloqueo;

//int contador;

int main(){
	
	pid_t pid;
	int np;
	int pipefd[NUM_PROC][2],edo_pipe;

	A=reservarMemoria();
	llenarArreglo(A);
	imprimirArreglo(A);

	//contador = 0;

	//Se agregó para tener exclusión mutua; Se inicializa en el proceso padre una vez
	//pthread_mutex_init(&bloqueo, NULL);
	
	printf("Probando procesos...\n");
	promedio=0;
/*	
	datos = reservarMemoria();
	llenarArreglo(datos);
	imprimirArreglo(datos);
*/	for( np = 0; np < NUM_PROC; np++){
		edo_pipe = pipe(&pipefd[np][0]);

		if( edo_pipe == -1 ){
			perror("Error al crear el pipe\n");
			exit(EXIT_FAILURE);
		}

                pid = fork();

                if( pid == -1){
                        perror("Error al crear el proceso hijo\n");
                        exit(EXIT_FAILURE);
                }

                if( !pid ){
                        proceso_hijo( np, &pipefd[np][0] );
                }
        }

	printf("--->%d\n", promedio >> 1);
	
	free(A);
	//pthread_mutex_destroy(&bloqueo);

	return 0;
}
void proceso_hijo( int np, int pipefd[] ){

	register int i;

	//Calculando el inicio y fin de cada bloque
	int tamBloque = N / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque;

	printf("Proceso hijo %d con pid %d  \n\n", np, getpid());

	close( pipefd[0] );

	for( i = iniBloque; i < finBloque; i++){
		promedio += A[i];
	}

	write( pipefd[1], &promedio, sizeof(int) );
	close(pipefd[1]);
	exit( np );
}

void proceso_padre( int pipefd[NUM_PROC][2] ){
	register int np;
        int pid, proc,suma_parcial;

	int tamBloque = N / NUM_PROC;
	int iniBloque;

        printf("Proceso padre con pid %d\n", getpid());
        for( np = 0; np < NUM_PROC; np++){
		close( pipefd[np][1] );
		//Si no me importa el orden de los resultados no hay problema con un solo pipe
		//Pero si si es necesario entonces se deben crear una tuberia para cada proceso

        pid = wait( &proc );
		proc = proc >> 8;

		//iniBloque = proc * tamBloque;
		read( pipefd[proc][0], suma_parcial, sizeof(int));
		promedio+=suma_parcial;
		printf("Proceso hijo %d, con pid %d\n\n", proc, pid);
		close( pipefd[proc][0] );

	}
	printf("Promedio----->%d\n", promedio>>5);
}

// void * funHilo(void *arg){
// 	register int i = 0;
// 	int nh = *(int*) arg;
// 	int suma_parcial=0;


// 	printf("Iniciando hilo %d \n", nh);
// 	for (i = nh; i < N; i+=NUM_HILOS)
// 	{
// 		//pthread_mutex_lock(&bloqueo);

// 		suma_parcial +=A[i];

// 		//pthread_mutex_unlock(&bloqueo);
// 	}

// 	pthread_mutex_lock(&bloqueo);
// 	promedio+=suma_parcial;
// 	pthread_mutex_unlock(&bloqueo);



// 	//Se agregó esto para crear una zona crítica y que los demás hilos no puedan acceder a ella mientras uno la esté ocupando
// 	//pthread_mutex_lock(&bloqueo);
// 	//contador++;	
	
// 	//while(--i); //Para que sea reentrante
// 	//sleep(5);	//Es no reentrante
// 	printf("Terminando hilo %d \n", nh);

// 	//pthread_mutex_unlock(&bloqueo);

// 	pthread_exit(arg);
// }

// *Ap : Contenido de Ap
//Con el cast se conoce los bytes que tiene que obtener a partir de la dirección de Ap

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

void imprimirArreglo(int *datos){
	register int i;

	for(i = 0; i < N; i++){

		if(!(i%16))
			printf("\n");

		printf("%3d ", datos[i]);
	}
	printf("\n");
}



/*#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#define NUM_HILOS 4
#define N 32

void * suma( void * arg);
void * resta( void * arg);
void * multi( void * arg);
void * divi( void * arg);


int *A,*B,*C;



int * reservarmemoria();
void llenararreglo(int * datos);
void imprimirarreglo(int *datos);
void producto(void );


int num=12,num2=12;
int main()
{
	A=reservarmemoria();
	llenararreglo(A);
	imprimirarreglo(A);


	B=reservarmemoria();
	llenararreglo(B);
	imprimirarreglo(B);


	C=reservarmemoria();

	producto();
	imprimirarreglo(C);
	free(A);
	free(B);
	free(C);










	return 0;

}

void * suma(void * arg){

	int *res=(int *)malloc(sizeof(int));
	*res=num+num2;
	pthread_exit((void*)  res);

}



void * resta(void * arg){


	int *res=(int *)malloc(sizeof(int));
	*res=num-num2;
	pthread_exit((void*)  res);
	
}

void * multi(void * arg){


	static int res;
	res=num*num2;
	pthread_exit((void*)  &res);

	
}

void * divi(void * arg){

	static int res;
	res=num/num2;
	pthread_exit((void*)  &res);

	
}



void llenararreglo(int *datos)
{
	register int i;
	srand(getpid());
	for ( i = 0; i <N; i++)
	{
		datos[i]=rand()%255;
	}
}


int * reservarmemoria()
{

	int *mem;
	mem=(int *) malloc(sizeof(int) * N);
	if( !mem)
	{
		perror("Error al asignar memoria ... \n");
		exit(EXIT_FAILURE);
	}
	return mem;


}



void producto(void )
{
	register int i;
	for (i = 0; i < N; i++)
	{	
		C[i]=A[i]*B[i];
	}
}



void imprimirarreglo(int *datos)
{
	register int i;
	for (i = 0; i < N; i++)
	{
		if(!(i%8))
		{
			printf("\n");
		}
		printf("%3d ", datos[i]);

	}
	printf("\n\n");
}








*/
/*

 Formas de guardas valores es :
 	- Reservar memoria en malloc
 	- Reservar una variable usamos el static 

*/
