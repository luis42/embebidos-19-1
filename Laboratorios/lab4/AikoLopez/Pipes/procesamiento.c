
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
	int pro = 0;

	for( register int i=0 ; i<N ; i++ )
	{
		pro = pro + datos[i];
	}

	return (int) pro/N;
}


void ordenarArreglo(int *datos) 
{
  int y,tmp;
  
  for(register int i = 1; i < N; i++) 
  {
      for(y = 0; y < N - i; y++) 
      {
        if(datos[y] > datos[y + 1]) 
        {
            tmp = datos[y];
            datos[y] = datos[y + 1];
            datos[y + 1] = tmp;
        }
      }
  }
}


