#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

#define NUM_PROC 4

void proceso_hijo( int np );
void proceso_padre(pid_t pid[]);
void ISRsw(int sig);

int main()
{
	
	pid_t pid[NUM_PROC];
	register int np;

	printf("Probando procesos...\n");

	if(signal(SIGUSR1,ISRsw) == SIG_ERR)
	{

		perror("Error al asignar manejador\n");
		exit(EXIT_FAILURE);
	}
	
	for( np = 0; np < NUM_PROC; np++ )
	{
		pid[np] = fork();
		if( pid[np] == -1 )
		{
			perror("Error al crear el proceso");
			exit(EXIT_FAILURE);
		}
		if( !pid[np] )
		{
			proceso_hijo( np );
		}
	}
	proceso_padre( pid ); 

	return 0;
}


void ISRsw(int sig)
{
	if(sig == SIGUSR1)
	{
		printf("-->Señal SIGUSR1 detectada...\n");
	}

}
void proceso_hijo( int np )
{
	printf("Proceso hijo %d con pid %d\n", np, getpid());

	pause();
	printf("Señal recibida en el proceso hijo : %d\n", np);
	//while(1);
	exit(np);
}

void proceso_padre(pid_t pid[])
{
	register int np;
	int status;
	pid_t pid1;

	printf("proceso padre con pid %d\n", getpid());
	for( np = 0; np < NUM_PROC; np++ )
	{
		printf("Proceso terminado ---> %d\n",np);
		kill(pid[np],SIGUSR1);
		sleep(2);
		//printf("proceso hijo %d terminado con PID %d\n", status>>8, pid);
	}

	for( np = 0; np < NUM_PROC; np++ )
	{
		pid1 = wait( &status );
		printf("proceso hijo %d terminado con PID %d\n", status>>8, pid1);
	}
}
