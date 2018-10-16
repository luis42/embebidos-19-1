#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include "defs.h"


void llenarSeno( float *datos )
{
	float f=1000, fs=45000;
	register int n;

	for( n=0 ; n < N ; n++ )
	{
		datos[n] = sinf(2*n*M_PI*f/fs);	
		//datos[n] = 1;	
	}
}

void llenarHamming( float *datos )
{
	register int n;

	//srand( getpid() );
	for( n=0 ; n<N ; n++ )
	{
		datos[n] = (1 - cosf( (2*n*M_PI)/(N) ) ) / 2;
	}
}

void llenarArreglo( float *datos )
{
	register int i;

	//srand( getpid() );
	for( i=0 ; i<N ; i++ )
	{
		datos[i] = rand() % 255;
	}
}

float * reservarMemoria()
{
	float *mem;
	mem = (float *)malloc( sizeof(float) * N );

	if( !mem )
	{
		perror("Error al asginar memoria...\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}


void imprimirArreglo( float *datos )
{
	register int i;

	for( i=0 ; i<N ; i++)
	{
		if ( !(i%16) )
		{
			printf("\n");
		}
		printf("%2f ", datos[i]);
	}
	printf("\n");
}

void guardaArreglo( float *datos )
{
	FILE *ap_arch;
	register int n;

	ap_arch=fopen("datos.dat","w");
	if( !ap_arch )
	{
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}
	for(n=0;n<N;n++)
	{
		fprintf(ap_arch,"%f \n",datos[n]);
	}

	fclose(ap_arch);
}

