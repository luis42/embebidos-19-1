
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

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

int promedioArreglo( int *datos )
{
	register int i , suma = 0;

	for( i=0 ; i<N ; i++ )
	{
		suma += datos[i];
	}

	return (int) suma/N;
}

void ordenarArreglo( int *datos )
{
	register int i , j , temp;	

	for( i=1 ; i<N ; i++ )
	{
		temp = datos[i];
		j = i-1;
		while( (datos[j]>temp) && (j>=0) )
		{
			datos[j+1] = datos[j];
			j--;
		}
		datos[j+1] = temp;
	}	
}

