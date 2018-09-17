#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#define NUM_PROC 4
#define MUESTRAS 512


void proceso_hijo( int np, int pipefd[] );
void proceso_padre( int pipefd[NUM_PROC][2]  );

float * reservarMemoria( void );
void imprimirArreglo( float datos[] );

void guarda_datos(float datos[], char *doc);

void llenar_seno(float seno[]);
void llenar_hamming(float hamming[]);

float *A, *B, *C;
float seno[MUESTRAS];
float hamming[MUESTRAS];

int main(){

	//Declaracion de variables
	pid_t pid;
	int pipefd[NUM_PROC][2], edo_pipe;
	register int np;

	//Reservar memoria
	A = reservarMemoria();
	B = reservarMemoria();
	C = reservarMemoria();

	//Llenar arreglos
	llenar_seno(A);
	llenar_hamming(B);

	//Imprimir los arreglos
	imprimirArreglo(A);
	guarda_datos(A, "seno.dat");
	imprimirArreglo(B);
	guarda_datos(B, "hamming.dat");

	for( np = 0; np < NUM_PROC; np++){
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

        proceso_padre( pipefd );

	imprimirArreglo(C);
	guarda_datos(C, "resultado.dat");
	free(A);
	free(B);
	free(C);

	return 0;
}

/* CON PROCESOS PERO ES MAS SENCILLO CON LA "Primera tecnica"
*/

void proceso_hijo( int np, int pipefd[] ){

	register int i;

	//Calculando el inicio y fin de cada bloque
	int tamBloque = MUESTRAS / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque;

	printf("Proceso hijo %d con pid %d  \n\n", np, getpid());

	close( pipefd[0] );

	for( i = iniBloque; i < finBloque; i++){
		C[i] = A[i] * B[i];
	}

	write( pipefd[1], C+iniBloque, sizeof(int)*tamBloque );
	close(pipefd[1]);
	exit( np );
}

void proceso_padre( int pipefd[NUM_PROC][2] ){
	register int np;
        int pid, proc;

	int tamBloque = MUESTRAS / NUM_PROC;
	int iniBloque;

        printf("Proceso padre con pid %d\n", getpid());
        for( np = 0; np < NUM_PROC; np++){
		close( pipefd[np][1] );
		//Si no me importa el orden de los resultados no hay problema con un solo pipe
		//Pero si si es necesario entonces se deben crear una tuberia para cada proceso

                pid = wait( &proc );
		proc = proc >> 8;

		iniBloque = proc * tamBloque;
		read( pipefd[proc][0], C+iniBloque, sizeof(int)*tamBloque);
		printf("Proceso hijo %d, con pid %d\n\n", proc, pid);
		close( pipefd[proc][0] );

	}
}

float * reservarMemoria( void ){
	float *mem;
	mem = (float *)malloc( sizeof(float)*MUESTRAS );
	if( !mem ){
		perror("Error al asignar memoria\n\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void imprimirArreglo( float datos[] ){
	register int n;
        for( n = 0; n < MUESTRAS; n++){
		if( !(n % 16))
			printf("\n");
        	printf("%0.3f ", datos[n]);
        }
	printf("\n\n");
}

void llenar_seno(float seno[]){
	float f = 1000, fs = 45000;
	register int n;

	for(n = 0; n < MUESTRAS; n++){
		seno[n] = sinf(2*n*M_PI*f/fs);
	}
}

void llenar_hamming(float hamming[]){
	register int n;

	for(n = 0; n < MUESTRAS; n++){
		hamming[n] = 0.54-(0.46*cosf(2*n*M_PI/(MUESTRAS-1)));
	}
}


void guarda_datos1(float datos[]){
	FILE *ap_arch;
	register int n;
	ap_arch = fopen("seno.dat","w");

	if(!ap_arch){
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	for(n = 0;n < MUESTRAS; n++){
		fprintf(ap_arch, "%f \n", datos[n]);
	}

	fclose(ap_arch);
}

void guarda_datos2(float datos[]){
	FILE *ap_arch;
	register int n;
	ap_arch = fopen("hamming.dat","w");

	if(!ap_arch){
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	for(n = 0;n < MUESTRAS; n++){
		fprintf(ap_arch, "%f \n", datos[n]);
	}

	fclose(ap_arch);
}

void guarda_datos3(float datos[]){
	FILE *ap_arch;
	register int n;
	ap_arch = fopen("resultado.dat","w");

	if(!ap_arch){
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	for(n = 0;n < MUESTRAS; n++){
		fprintf(ap_arch, "%f \n", datos[n]);
	}

	fclose(ap_arch);
}


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