#include <stdio.h>
#include <stdlib.h>
#include "imagen.h"
unsigned char * RGBtoGray(unsigned char *imagenRGB,uint32_t width,uint32_t height);
void GraytoRGB(unsigned char *imagenGray,unsigned char *imagenRGB,uint32_t width,uint32_t height);
int main()
{
	

	bmpInfoHeader info;
	unsigned char *imagenRGB,*imagenGray;

	imagenRGB = abrirBMP("hoja.bmp",&info);

	displayInfo( &info);
	imagenGray=RGBtoGray(imagenRGB ,info.width,info.height);
	GraytoRGB(imagenGray , imagenRGB , info.width,info.height);
	printf("Guardando\n");
	guardarBMP("hojaGra2.bmp", &info , imagenRGB);
	free(imagenRGB);
	free(imagenGray);



	return 0;
}


unsigned char * RGBtoGray(unsigned char *imagenRGB,uint32_t width,uint32_t height)
{

	register int x,y;
	int indiceRGB,indiceGray;
	unsigned char grayLevel;
	unsigned char *imagenGray;
	imagenGray=(unsigned char*) malloc(width*height*sizeof(unsigned char));
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


