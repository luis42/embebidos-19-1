
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "imagen.h"


unsigned char * reservarMemoria( uint32_t width , uint32_t height )
{
	unsigned char *imagen;	

	imagen = (unsigned char *)malloc(width*height*sizeof(unsigned char));

	if( imagen == NULL )
	{
		perror("Error al asignar memoria");
		exit(EXIT_FAILURE);
	}

	return imagen;
}


