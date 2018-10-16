
#ifndef PROCESO_H
#define PROCESO_H

void procesoHijo( int np , int *datos , int pipefd[] );
void procesoPadre( int pipefd[] );

#endif
