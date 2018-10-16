#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROC 4
#define N 32

int *A , *B , *C;

int * reservarMemoria();
void llenarArreglo( int *datos );
void imprimirArreglo( int *datos );

void procesoHijo( int np , int pipefd[] );
void procesoPadre( int pipefd[NUM_PROC][2] );


int main()
{
	pid_t pid;
	int np;
	int pipefd[NUM_PROC][2] , pipe_status;

	srand( getpid() );

	printf("Probando hilos...\n");

	A = reservarMemoria();
	llenarArreglo( A );
	imprimirArreglo( A );

	B = reservarMemoria();
	llenarArreglo( B );
	imprimirArreglo( B );

	C = reservarMemoria();

	printf("Probando procesos...");
	
	
	for( np = 0 ; np < NUM_PROC ; np++ )
	{

		pipe_status = pipe( &pipefd[np][0] );
		if( pipe_status == -1)
		{
			perror("Error al crear la tuberia...\n");
			exit( EXIT_FAILURE );
		}

		pid = fork();
		if( pid == -1 )
		{
			perror("No se creo el proceso...\n");
			exit( EXIT_FAILURE );
		}
		if( !pid )
		{
			procesoHijo( np ,  &pipefd[np][0] );
		}

	}
	procesoPadre( pipefd );
	
	imprimirArreglo( C );
	free( A );	
	free( B );	
	free( C );	

	return 0;
}

void * funHilo( void *arg )
{
	register int i=0;
	int nh = *(int *)arg;

	for( i=nh ; i<N ; i+=NUM_PROC )
	{
		C[i] = A[i] * B[i];
	}
	pthread_exit( arg );
}


void procesoPadre( int pipefd[NUM_PROC][2] )
{
	int np , npc , tamBloque = N / NUM_PROC , iniBloque;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d \n" , getpid() );


	for( np = 0 ; np<NUM_PROC ; np++)
	{
		pid_hijo = wait( &npc );
		npc = npc >> 8;
		close( pipefd[npc][1] );
		iniBloque = npc * tamBloque;
		read( pipefd[npc][0] , C+iniBloque , sizeof(int)*tamBloque );
		printf("Proceso hijo %d terminado pid %d \n" , npc , pid_hijo );
		close( pipefd[npc][0] );
	}
}


void procesoHijo( int np , int pipefd[] )
{
	register int i;	

	int tamBloque = N / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque; 


	printf("Proceso hijo %d ejecutado con pid %d \n" , np , getpid() );
	close( pipefd[0] );

	for( i=iniBloque ; i<finBloque ; i++ )
	{
		C[i] = A[i] * B[i];
	}	

	write( pipefd[1] , C + iniBloque , sizeof(int)*tamBloque );
	close( pipefd[1] );

	exit( np );
}


void llenarArreglo( int *datos )
{
	register int i;

	//srand( getpid() );
	for( i=0 ; i<N ; i++ )
	{
		datos[i] = rand() % 255;
	}
}

int * reservarMemoria()
{
	int *mem;
	mem = (int *)malloc( sizeof(int) * N );

	if( !mem )
	{
		perror("Error al asginar memoria...\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}


void imprimirArreglo( int *datos )
{
	register int i;

	for( i=0 ; i<N ; i++)
	{
		if ( !(i%16) )
		{
			printf("\n");
		}
		printf("%5d ", datos[i]);
	}
	printf("\n");
}

