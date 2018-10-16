#ifndef PROCESOS_H
#define PROCESOS_H

void procesoPadre(int pipefd[]);
void procesoHijo(int np, int *datos, int pipefd[]);

#endif