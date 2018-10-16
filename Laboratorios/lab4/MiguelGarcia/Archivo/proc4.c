
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define NUM_PROC 2
#define N 32

int * reservarMemoria();
void llenarArreglo( int *datos );
void imprimirArreglo( int *datos );
int mayorArreglo( int *datos );
int menorArreglo( int *datos );

void procesoHijo( int np , int *datos );
void procesoPadre();

int main()
{
	pid_t pid;
	int *datos , np;
	
	datos = reservarMemoria();
	llenarArreglo( datos );
	imprimirArreglo( datos );

	printf("Probando procesos...");

	for( np=0 ; np < NUM_PROC ; np++ )
	{
		pid = fork();

		if( pid == -1 )
		{
			perror("No se creo el proceso...\n");
			exit( EXIT_FAILURE );		
		}
		if( !pid )
		{
			procesoHijo( np , datos );
		}
	}
	procesoPadre();
	return 0;
}


void procesoPadre()
{
	int np , numero;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d \n" , getpid() );	
	
	for( np = 0 ; np < NUM_PROC ; np++ )
	{
		pid_hijo = wait( &numero );
		printf("Proceso hijo terminado con pid %d y retorno %d \n", pid_hijo , numero>>8 );	
	}
}

void procesoHijo( int np , int *datos)
{
	int mayor , menor;

	printf("Proceso hijo %d ejecutado con pid %d \n" , np , getpid() );	
	//while(1);
	
	if( np == 0 )
	{
		mayor = mayorArreglo( datos );
		exit( mayor );
	}	
	else 
	{
		if( np == 1 )
		{
			menor = menorArreglo( datos );
			exit( menor );
		}
	}
}

int menorArreglo( int *datos )
{
	register int i , menor;
	
	menor = datos[0];
	for( i=1 ; i<N ; i++)
	{
		if( datos[i] < menor )
		{
			menor = datos[i];
		}
	}
	return menor;
}

int mayorArreglo( int *datos )
{
	register int i , mayor;
	
	mayor = datos[0];
	for( i=1 ; i<N ; i++)
	{
		if( datos[i] > mayor )
		{
			mayor = datos[i];
		}
	}
	return mayor;
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
		printf("%3d ", datos[i]);
	}
	printf("\n");
}

void llenarArreglo( int *datos )
{
	register int i;

	srand( getpid() );
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

