#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "datos.h"
#include "hilo.h"
#include "defs.h"

int *resultado, nhs[NUM_HILOS], *datos;
int mayor, menor, promedio;
pthread_mutex_t bloqueo;

int main(){
	register int nh;

	datos = reservarMemoria();
	
	srand(getpid());

	llenarArreglo( datos );
	imprimirArreglo( datos );


	//Declaracion del hilo
	pthread_t tids[NUM_HILOS];

	//Inicializar el bloqueo
	pthread_mutex_init( &bloqueo, NULL );

	//Creacion del hilo
	for(nh = 0; nh < NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create( &tids[nh], NULL, funHilo, (void *)&nhs[nh] );
	}

	//Esperar al hilo
	for( nh = 0; nh < NUM_HILOS; nh++){
		pthread_join( tids[nh], (void **)&resultado );
		//Imprimir resultados
		if( *resultado == 3 ){
			printf("\nTerminando el hilo %d ordenando\n", nh);
			imprimirArreglo( datos );
		}
		else{
			printf("\nTerminando el hilo %d con resultado %d \n", nh, *resultado);
		}
	}

	//imprimirArreglo( datos );

	//Destruyendo el bloqueo
	pthread_mutex_destroy( &bloqueo );
	return 0;

}
