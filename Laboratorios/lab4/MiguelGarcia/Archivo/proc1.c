
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	float num1 = 23 , num2 = 12 , suma , resta;
	pid_t pid , pid_hijo;
	int estado;

	printf("Probando procesos...");

	pid = fork();

	if( !pid )
	{
		printf("Proceso hijo ejecutado con pid %d" , getpid() );	
		suma = num1 + num2;
		printf("La suma es: %f\n" , suma);
		exit(0);
	}
	else
	{
		//sleep(5);
		printf("Proceso padre ejecutado con pid %d" , getpid() );	
		resta = num1 - num2;
		printf("La resta es: %f\n" , resta);
		pid_hijo = wait( &estado );
		printf("Proceso hijo terminado con pid %d \n", pid_hijo );
	}

	return 0;
}
