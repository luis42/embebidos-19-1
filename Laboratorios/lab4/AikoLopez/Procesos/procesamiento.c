
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

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



















/*int * ordenaArreglo(int * datos){
    int * d = reservarMemoria();
    memcpy ( d, datos, N*sizeof(int));
    for(register int i = 0; i < N; i++){
        for(register int j = 0; j < N-1-i; j++){
            if (d[j] > d[j+1]) {
                int tmp = d[j];
                d[j] = d[j+1];
                d[j+1] = tmp;
            }
        }
    }
    return d;
}
int promedioArreglo(int * datos){
    int promedio=0;
    for(register int i = 0; i< N; i++){
        promedio = promedio + datos[i];
    }
    promedio = promedio << 4;
    return promedio;
}*/






