#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>


void ISRsw(int sig);
int main(int argc, char const *argv[])
{
	
	if(signal( SIGINT ,ISRsw) == SIG_ERR)
	{
		perror("Error al crear la ISR");
		exit(EXIT_FAILURE);
	}
	if(signal( SIGTERM ,ISRsw) == SIG_ERR)
	{
		perror("Error al crear la ISR");
		exit(EXIT_FAILURE);
	}
	printf("Ejecutando\n");
	while(1) pause();





	return 0;
}

void ISRsw(int sig)
{
	if(sig == SIGINT)
		printf("Señal detectada SIGINT\n");
	if(sig == SIGTERM)
		printf("Señal detectada de kill SIGTERM\n");
}