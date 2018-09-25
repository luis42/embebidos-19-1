//conversion.c Para convertir una imagen de RGB a escala de grises

#include <stdio.h>
#include <stdlib.h>

#include "imagen.h"

void GraytoRGB(unsigned char *imagenGray, unsigned char *imagenRGB, uint32_t width, uint32_t height);
unsigned char * RGBtoGray(unsigned char *imagenRGB, uint32_t width, uint32_t height);

int main(){
	
	bmpInfoHeader info;
	unsigned char *imagenRGB, *imagenGray;


	//Abrir imagen
	imagenRGB = abrirBMP("../img/hoja.bmp", &info);
	displayInfo(&info);

	//Convertir a nivel de gris
	imagenGray = RGBtoGray(imagenRGB, info.width, info.height);

	//procesamiento...

	//Para guardar una imagen otra vez en RGB, se pone el mismo valor de nivel de gris en los tres componentes (R, G, B), quedando en formato RGB pero en escala de grises. Se regresa al formato RGB para que el visor pueda reconocerlo. Se puede modificar la cabezera para indicar que cada pixel es de 1 byte.
	GraytoRGB(imagenGray, imagenRGB, info.width, info.height);

	//Guardar imagen en escala de grises
	guardarBMP("hojaGray.bmp", &info, imagenRGB); //Se utiliza el mismo buffer de imagenRGB que ya estaba reservado

	free(imagenRGB);
	free(imagenGray);

	return 0;
}

//Se utiliza el mismo buffer de imagenRGB que ya estaba reservado
void GraytoRGB(unsigned char *imagenGray, unsigned char *imagenRGB, uint32_t width, uint32_t height){

	register int x, y;
	int indiceRGB, indiceGray;

	if(imagenGray == NULL){
		perror("Error al asignar memoria.\n");
		exit(EXIT_FAILURE);
	}

	for(y = 0; y < height; y++){
		for(x = 0; x < width; x++){
			indiceGray = y * width + x; //Para desplazarse en el arreglo
			indiceRGB = indiceGray * 3; //por que cada pixel ocupa 3 bytes

			imagenRGB[indiceRGB] = imagenGray[indiceGray];
			imagenRGB[indiceRGB+1] = imagenGray[indiceGray];
			imagenRGB[indiceRGB+2] = imagenGray[indiceGray];
		}
	}
}

unsigned char * RGBtoGray(unsigned char *imagenRGB, uint32_t width, uint32_t height){

	register int x, y;
	int indiceRGB, indiceGray;
	unsigned char grayLevel;
	unsigned char *imagenGray;

	//Arreglo a reservar para la nueva imagen
	imagenGray = (unsigned char *) malloc(width * height * sizeof(unsigned char));

	if(imagenGray == NULL){
		perror("Error al asignar memoria.\n");
		exit(EXIT_FAILURE);
	}

	//'x' y 'y' recorren pixeles pero cada pixel ocupa 3 bytes en memoria
	for(y = 0; y < height; y++){
		for(x = 0; x < width; x++){
			indiceGray = y * width + x; //Para desplazarse en el arreglo
			indiceRGB = indiceGray * 3; //por que cada pixel ocupa 3 bytes

			grayLevel = (imagenRGB[indiceRGB] + imagenRGB[indiceRGB+1] + imagenRGB[indiceRGB+2]) / 3;

			imagenGray[indiceGray] = grayLevel;
		}
	}

	return imagenGray;
}

/*
	====IMAGEN RGB====

3 bytes representan un pixel.

|----------width------------|
_R G B | R G B | R G B | R G B ...
.
.
.
.height
.
.
.
_

por ejemplo, una imagen de 200x100 son pixeles, entonces hay que multiplicar por 3 para conocer la cantidad de bytes.


	====IMAGEN GRAY====

1 byte representa un pixel.

|----------width------------|
_NG | NG | NG | NG | NG | ...
.
.
.
.height
.
.
.
_

NG (nivel de gris) = R + G + B / 3

*/