
#ifndef PROCESO_H
#define PROCESO_H

void procesoHijo( int np , int pipefd[] );
void procesoPadre( int pipefd[NUM_PROC][2] );

#endif
