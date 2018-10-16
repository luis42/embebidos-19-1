
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <math.h>
#include "defs.h"
#include "imagen.h"
#include "datos.h"

extern bmpInfoHeader info;
extern unsigned char *imagenRGB , *imagenGray , *imagenFiltro;

void * filtroImagen( void *arg )
{
	int nh = *(int *)arg;
	int tamBloqueY = info.height  / NUM_HILOS;
	int iniBloque = nh * tamBloqueY;
	int finBloque = iniBloque + tamBloqueY;
	
	if( nh == (NUM_HILOS-1) )
	{
		finBloque -= DIMASK;
	}

	register int x , y , ym , xm;
	int conv1 , conv2 , indice , indicem , magnitud , tamBloqueX;

	char gradF[] =
		{ 1 , 0 , -1 ,
		  2 , 0 , -2 ,
		  1 , 0 , -1 }; 	

	char gradC[] =
		{ -1 , -2 , -1 ,
		   0 ,  0 ,  0 ,
		   1 ,  2 ,  1 }; 	

	tamBloqueX = info.width - DIMASK;

	for( y=iniBloque ; y <= finBloque ; y++)
	{
		for( x=0 ; x <= tamBloqueX ; x++ )
		{
			conv1 = 0;
			conv2 = 0;
			indicem = 0;

			for( ym = 0 ; ym < DIMASK ; ym++ )
			{
				for( xm = 0 ; xm < DIMASK ; xm++)
				{
					indice = ((y+ym)*info.width +(x+xm));
					conv1 += imagenGray[indice] * gradF[indicem];
					conv2 += imagenGray[indice] * gradC[indicem++];
				}
			}
			conv1 = conv1 / 4;
			conv2 = conv2 / 4;

			magnitud = sqrt( conv1*conv1 + conv2*conv2 );

			indice = ( (y+1) * info.width + (x+1) );
			imagenFiltro[indice] = magnitud;
		}
	}

	pthread_exit((void *)&nh);	
}

void * GraytoRGB( void *arg )
{
	register int x , y;
	int indiceRGB , indiceGray;

	int nh = *(int *)arg;
	int tamBloqueY = info.height  / NUM_HILOS;
	int iniBloque = nh * tamBloqueY;
	int finBloque = iniBloque + tamBloqueY;

	for( y = iniBloque ; y < finBloque ; y++ )
	{
		for( x=0 ; x < info.width ; x++)
		{
			indiceGray = ( y * info.width + x );
			indiceRGB = indiceGray * 3;

			imagenRGB[indiceRGB+0] = imagenFiltro[indiceGray];
			imagenRGB[indiceRGB+1] = imagenFiltro[indiceGray];
			imagenRGB[indiceRGB+2] = imagenFiltro[indiceGray];
		}
	}

	pthread_exit((void *)&nh);	
}

void * RGBtoGray( void * arg )
{
	register int x , y;
	int indiceRGB , indiceGray;
	unsigned char grayLevel;

	int nh = *(int *)arg;
	int tamBloqueY = info.height  / NUM_HILOS;
	int iniBloque = nh * tamBloqueY;
	int finBloque = iniBloque + tamBloqueY;

	for( y = iniBloque ; y < finBloque ; y++ )
	{
		for( x=0 ; x < info.width ; x++)
		{
			indiceGray = ( y * info.width + x );
			indiceRGB = indiceGray * 3;
			grayLevel = ( (30*imagenRGB[indiceRGB]) + 59*imagenRGB[indiceRGB+1] + 11*imagenRGB[indiceRGB+2] )/100;
			imagenGray[indiceGray] = grayLevel;	
		}
	}

	pthread_exit((void *)&nh);
}


void * brilloImagen( void * arg )
{
	register int p;
	unsigned short int pixel;

	int nh = *(int *)arg;
	int tamBloqueY = info.width * info.height  / NUM_HILOS;
	int iniBloque = nh * tamBloqueY;
	int finBloque = iniBloque + tamBloqueY;

	for( p=iniBloque ; p < finBloque ; p++ )
	{
		pixel = imagenGray[p] + 80;
		imagenGray[p] = (pixel >255 )? 255 : (unsigned char)pixel;
	}

	pthread_exit((void *)&nh);
}


