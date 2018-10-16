
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include "defs.h"
#include "datos.h"
#include "imagen.h"
#include "hilos.h"

bmpInfoHeader info;
unsigned char *imagenRGB , *imagenGray , *imagenFiltro;

int main()
{
	printf("Imagen con Filtro Sobel\n");

	int nhs[NUM_HILOS] , *res;
	register int nh;
	pthread_t tids[NUM_HILOS];

		//Archivo
	imagenRGB = abrirBMP( "calle1.bmp" , &info );
	displayInfo( &info );

	imagenGray = reservarMemoria( info.width , info.height );

		//RGBtoGray
	printf("Abriendo imagen RGB\n");

	for( nh = 0 ; nh < NUM_HILOS ; nh++ )
	{
		nhs[nh] = nh;
		pthread_create( &tids[nh] , NULL , RGBtoGray , (void *)&nhs[nh] );
	}

	for( nh = 0 ; nh < NUM_HILOS ; nh++ )
	{
		pthread_join( tids[nh] , (void **)&res );
		//printf("Hilo %d terminado \n" , *res );
	}

	
		//procesamiento
	printf("Procesando...\n");
	//brilloImagen( imagenGray , info.width , info.height );
	imagenFiltro = reservarMemoria( info.width , info.height);
	
	for( nh = 0 ; nh < NUM_HILOS ; nh++ )
	{
		nhs[nh] = nh;
		pthread_create( &tids[nh] , NULL , filtroImagen , (void *)&nhs[nh] );
	}

	for( nh = 0 ; nh < NUM_HILOS ; nh++ )
	{
		pthread_join( tids[nh] , (void **)&res );
		//printf("Hilo %d terminado \n" , *res );
	}
	

		//GraytoRGB ans Save
	printf("Guardando imagen\n");

	for( nh = 0 ; nh < NUM_HILOS ; nh++ )
	{
		nhs[nh] = nh;
		pthread_create( &tids[nh] , NULL , GraytoRGB , (void *)&nhs[nh] );
	}

	for( nh = 0 ; nh < NUM_HILOS ; nh++ )
	{
		pthread_join( tids[nh] , (void **)&res );
		//printf("Hilo %d terminado \n" , *res );
	}

	guardarBMP( "calle1Sobel.bmp" , &info , imagenRGB );

	free( imagenRGB );
	free( imagenGray );
	free( imagenFiltro );

	return 0;
}


