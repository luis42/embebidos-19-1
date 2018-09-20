#include <stdio.h>
#include <stdlib.h>
#include "imagen.h"

int main()
{
	

	bmpInfoHeader info;
	unsigned char *imagen;

	imagen = abrirBMP("hoja.bmp",&info);

	displayInfo( &info);

	free(imagen);

	return 0;
}