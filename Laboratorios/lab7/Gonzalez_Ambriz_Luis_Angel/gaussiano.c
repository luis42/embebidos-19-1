#include <stdio.h>
#include <stdlib.h>
#include "imagen.h"
#define DIMASK 5


unsigned char * RGBtoGray(unsigned char *imagenRGB,uint32_t width,uint32_t height);
void GraytoRGB(unsigned char *imagenGray,unsigned char *imagenRGB,uint32_t width,uint32_t height);
void brilloImagen( unsigned char *imagenGray,uint32_t width,uint32_t height);
unsigned char * reservarMemoria(uint32_t width,uint32_t height);
void filtroImagen(unsigned char * imagenGray,unsigned char * imagenFiltro,uint32_t width,uint32_t height);


int main()
{
	

	bmpInfoHeader info;
	unsigned char *imagenRGB,*imagenGray,*imagenFiltro;

	imagenRGB = abrirBMP("calle1.bmp",&info);

	displayInfo( &info);
	imagenGray=RGBtoGray(imagenRGB ,info.width,info.height);
//	brilloImagen(imagenGray,info.width,info.height);
	imagenFiltro=reservarMemoria(info.width,info.height);
	filtroImagen(imagenGray,imagenFiltro,info.width,info.height);
	
	GraytoRGB(imagenFiltro , imagenRGB , info.width , info.height );
	printf("Guardando\n");

	guardarBMP("calle3.bmp", &info , imagenRGB);

	free(imagenRGB);
	free(imagenGray); 
	free(imagenFiltro);



	return 0;
}

void filtroImagen(unsigned char * imagenGray,unsigned char * imagenFiltro,uint32_t width,uint32_t height)
{

	register int y,x,ym,xm;

	int indiceGray,conv,indicem;
	unsigned char mascara[]={
							1, 4, 7, 4, 1,
							4,16,26,16, 4,
							7,26,41,26, 7,
							4,16,26,16, 4,
							1, 4, 7, 4, 1};
	for (y = 0; y <= height-DIMASK ; y++)
	{
		for (x = 0; x <= width-DIMASK ; x++)
		{
			conv=0;
			indicem=0;
			for (ym= 0; ym < DIMASK; ym++)
			{

				for (xm = 0; xm < DIMASK; xm++)
				{
					indiceGray = ((y + ym) * width + (x + xm ));
					conv+=imagenGray[indiceGray]*mascara[indicem++];

				}
			}
				conv=conv/213;
				indiceGray=((y+1)*width + (x+1));
				imagenFiltro[indiceGray]=conv;
				
			
			
		}
	}


}

unsigned char * reservarMemoria(uint32_t width,uint32_t height)
{
	unsigned char *imagenGray;
	imagenGray=(unsigned char*) malloc(width*height*sizeof(unsigned char));
	if(imagenGray == NULL)
	{
		perror("Error al asignar memoria");
		exit(EXIT_FAILURE);
	}
	return imagenGray;

}

void brilloImagen( unsigned char *imagenGray,uint32_t width,uint32_t height)
{
	register int p;
	unsigned short int pixel;
	for (p = 0; p < width*height; p++)
	{

		pixel=imagenGray[p] + 80;
		imagenGray[p] = (pixel > 255 )  ? 255 : (unsigned char)pixel;

	}
}





unsigned char * RGBtoGray(unsigned char *imagenRGB,uint32_t width,uint32_t height)
{

	register int x,y;
	int indiceRGB,indiceGray;
	unsigned char grayLevel;
	unsigned char *imagenGray;
	//imagenGray=(unsigned char*) malloc(width*height*sizeof(unsigned char));
	imagenGray=reservarMemoria(width,height);
	if(imagenGray == NULL)
	{
		perror("Error al asignar memoria");
		exit(EXIT_FAILURE);
	}

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{

			indiceGray = (y*width +x);
			//indiceRGB  = indiceGray * 3;
			indiceRGB  =  ((indiceGray << 1) + indiceGray);
			grayLevel  = (30*imagenRGB[indiceRGB] + 59*imagenRGB[indiceRGB+1] + 11*imagenRGB[indiceRGB+2])/100;
			imagenGray[indiceGray] =grayLevel;
		}

	}


	return imagenGray;

}




void GraytoRGB(unsigned char *imagenGray,unsigned char *imagenRGB,uint32_t width,uint32_t height)
{

	register int x,y;
	int indiceRGB,indiceGray;


	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{

			indiceGray = (y*width +x);
			indiceRGB  = indiceGray * 3;
			imagenRGB[indiceRGB+0] = imagenGray[indiceGray];
			imagenRGB[indiceRGB+1] = imagenGray[indiceGray];
			imagenRGB[indiceRGB+2] = imagenGray[indiceGray];
		}

	}


}


