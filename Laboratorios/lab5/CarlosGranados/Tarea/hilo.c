#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "defs.h"
#include "procesamiento.h"
#include "datos.h"

extern int *datos;
extern pthread_mutex_t bloqueo;
extern int mayor, menor, promedio;

void * funHilo( void *arg ){
	int oper = *(int *)arg;

	switch( oper ){
		case 0:
			pthread_mutex_lock( &bloqueo );
			mayor = mayorArreglo( datos );
			pthread_mutex_unlock( &bloqueo );
			pthread_exit( &mayor );
		break;
		case 1:
			pthread_mutex_lock( &bloqueo );
			menor = menorArreglo( datos );
			pthread_mutex_unlock( &bloqueo );
			pthread_exit( &menor );
		break;
		case 2:
			pthread_mutex_lock( &bloqueo );
			promedio = promedioArreglo( datos );
			pthread_mutex_unlock( &bloqueo );
			pthread_exit( &promedio );
		break;
		case 3:
			pthread_mutex_lock( &bloqueo );
			ordenarArreglo( datos );
			pthread_mutex_unlock( &bloqueo );
			pthread_exit( arg );
		break;
		default:
			pthread_exit( arg );
		break;
	}
}