#include <stdio.h>
#include "defs.h"

int mayorArreglo(int *datos)
{
	register int i, mayor;
	
	mayor = datos[0];
	for(i=1; i<N; i++)
	{
		if(datos[i] > mayor)
			mayor = datos[i];
	}
	return mayor;
}

int menorArreglo(int *datos)
{
	register int i, menor;
	
	menor = datos[0];
	for(i=1; i<N; i++)
	{
		if(datos[i] < menor)
			menor = datos[i];
	}
	return menor;
}

int promedioArreglo (int *datos)
{
	register int i, suma = datos[0];
	
	for (i=1; i<N; i++)
	{
		suma += datos[i];
	}
	return (suma / N);
}

void ordenarArreglo(int *datos)
{
	int pasada, aux, i;
	
	for(pasada = 0; pasada < N; pasada++)
	{
		for(i=0; i<=N-2; i++)
		{
			if(datos[i] > datos[i+1])
			{
				aux = datos[i];
				datos[i] = datos[i+1];
				datos[i+1] = aux;
			}
		}
	}
}