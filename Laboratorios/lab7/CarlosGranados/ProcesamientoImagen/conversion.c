//Utiliando el filtro Gaussiano

#include <stdio.h>
#include <stdlib.h>

#include "imagen.h"

#define DIMASK 5

void GraytoRGB(unsigned char *imagenGray, unsigned char *imagenRGB, uint32_t width, uint32_t height);
unsigned char * RGBtoGray(unsigned char *imagenRGB, uint32_t width, uint32_t height);
void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height);
void filtroImagen(unsigned char *imagenGray, unsigned char *imagenFiltro, uint32_t width, uint32_t height);
unsigned char * reservarMemoria(uint32_t width, uint32_t height);

int main(){
	
	bmpInfoHeader info;
	unsigned char *imagenRGB, *imagenGray, *imagenFiltro;


	//Abrir imagen
	imagenRGB = abrirBMP("../img/huella1.bmp", &info);
	displayInfo(&info);

	//Convertir a nivel de gris
	imagenGray = RGBtoGray(imagenRGB, info.width, info.height);

	//procesamiento...

	//brilloImagen(imagenGray, info.width, info.height);

	imagenFiltro = reservarMemoria(info.width, info.height);
	filtroImagen(imagenGray, imagenFiltro, info.width, info.height);

	//Para guardar una imagen otra vez en RGB, se pone el mismo valor de nivel de gris en los tres componentes (R, G, B), quedando en formato RGB pero en escala de grises. Se regresa al formato RGB para que el visor pueda reconocerlo. Se puede modificar la cabezera para indicar que cada pixel es de 1 byte.
	GraytoRGB(imagenFiltro, imagenRGB, info.width, info.height);

	//Guardar imagen en escala de grises
	guardarBMP("huella1FGaussiano.bmp", &info, imagenRGB); //Se utiliza el mismo buffer de imagenRGB que ya estaba reservado

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
	imagenGray = reservarMemoria(width, height);

	if(imagenGray == NULL){
		perror("Error al asignar memoria.\n");	
		exit(EXIT_FAILURE);
	}

	//'x' y 'y' recorren pixeles pero cada pixel ocupa 3 bytes en memoria
	for(y = 0; y < height; y++){
		for(x = 0; x < width; x++){
			indiceGray = y * width + x; //Para desplazarse en el arreglo
			indiceRGB = indiceGray * 3; //por que cada pixel ocupa 3 bytes

			//Utilizando un método de iluminación ponderado. Cada color tiene diferente longitud de onda y por lo tanto se ponderan los valores de R G B para tener un balance.
			// 0.3 R, 0.59 G, 0.11 B
			// Pero para no enviar al coprocesador matemático se utiliza en forma entera como:
			// 30 R, 59 G, 11 B y luego entre 100, de esta manera manejamos numeros enteros
			grayLevel = (30 * imagenRGB[indiceRGB] + 
						59 * imagenRGB[indiceRGB+1] + 
						11 * imagenRGB[indiceRGB+2]) / 100;

			imagenGray[indiceGray] = grayLevel;
		}
	}

	return imagenGray;
}

void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height){
	register int p;
	unsigned short int pixel;

	//Otra manera de recorrer los pixeles, recorriendo el arreglo lineal
	for(p = 0; p < width*height; p++){
		pixel = imagenGray[p] + 80; //Se suma una constante para aumentar el brillo
		imagenGray[p] = (pixel > 255)?255:pixel; //Operador ternario, se debe comparar porque se va a almacenar máximo 255, por lo que si al sumar la constante excede el 255, se debe guardar 255
	}
}

unsigned char * reservarMemoria(uint32_t width, uint32_t height){
	unsigned char *imagen;

	//Arreglo a reservar para la nueva imagen
	imagen = (unsigned char *) malloc(width * height * sizeof(unsigned char));

	if(imagen == NULL){
		perror("Error al asignar memoria.\n");
		exit(EXIT_FAILURE);
	}

	return imagen;
}

void filtroImagen(unsigned char *imagenGray, unsigned char *imagenFiltro, uint32_t width, uint32_t height){
	register int x, y, xm, ym, indicem;
	int conv, indice;

	//Se utiliza una máscara de 3x3. Hay que convolucionar los valores de la imagen con los valores de la máscara
	//Utilizando el filtro Gaussiano
	unsigned char mascara[] = 
							{1,  4,  7,  4, 1,
	 						 4, 16, 26, 16, 4,
	 						 7, 26, 41, 26, 7,
	 						 4, 16, 26, 16, 4,
	 						 1,  4,  7,  4, 1,};

	//Se resta la dimensión de la máscara porque viéndolo como cuadrícula, se 
	for(y = 0; y <= height-DIMASK; y++){
		for(x = 0; x <= width-DIMASK; x++){ 
			//Para recorrer la máscara
			conv = 0;
			indicem = 0;

			for(ym = 0; ym < DIMASK; ym++){
				for(xm = 0; xm < DIMASK; xm++){
					indice = (y+ym)*width+(x+xm);
					conv += imagenGray[indice] * mascara[indicem++];
				}
			}

			conv = conv / 273; //Coeficiente dado por el filtro Gaussiano
			indice = ((y+1)*width + (x+1));
			imagenFiltro[indice] = conv;

		}
	}

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