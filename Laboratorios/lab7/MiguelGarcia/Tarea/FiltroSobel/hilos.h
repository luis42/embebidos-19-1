#include <stdint.h>
#include <stdio.h>
#include "imagen.h"

#ifndef HILO_H
#define HILO_H

void * filtroImagen( void *arg );
void * GraytoRGB( void * arg );
void * RGBtoGray( void * arg );
void * brilloImagen( void * arg );


#endif
