#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void ISRsw(int sig);

int main(){

	if (signal(SIGINT, ISRsw) == SIG_ERR){
		perror("Error al crear la ISR");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGTERM, ISRsw) == SIG_ERR){
		perror("Error al crear la ISR");
		exit(EXIT_FAILURE);
	}

	printf("Señal ejecutada \n");

	while(1) pause(); //pause: el proceso se queda esperando señales

	return 0;
}

void ISRsw(int sig){
	//Si detecta la señal ^C o KILL -SIGINT <numproceso> o KILL -SIGTERM <numproceso> muestra el mensaje. KILL -SIGKILL <numproceso> mata el proceso sin importar 
	if(sig == SIGINT){
		printf("Señal SIGINT detectada, No voy a terminar \n");
	}else if(sig == SIGTERM){ 
		printf("Señal SIGTERM detectada, No voy a terminar \n");
	}
}


//Con KILL se mandan señales (entre ellas una para matar procesos)