#include "defs.h"
#ifndef PROCESOS_H
#define PROCESOS_H

void proceso_hijo( int np, int pipefd[] );
void proceso_padre( int pipefd[]  );

#endif