#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "defs.h"

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

int promedioArreglo( int datos[] ){
	register int n;
   	int suma = 0;
	int  promedio;

        for( n = 0; n < N; n++){
		suma += datos[n];
        }

	promedio = suma/N;
        return promedio;
}


void ordenaArreglo(int *datos)
{
	register int i, j, menor;
	for (i = 0; i <N ; i++)
	{
		for (j = 0; j < N; j++)
		{
			if(!(datos[i]>datos[j]))
			{
				menor=datos[i];
				datos[i]=datos[j];
				datos[j]=menor;
			}
		}


	}
	//Método de Selección

	
}


