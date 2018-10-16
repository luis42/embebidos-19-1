#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
	pid_t pid, pid_hijo;
	float num1=23, num2=12, s,r;
	int estado;
	
	printf("Probando procesos \n");

	pid = fork();
	if(!pid){
	       printf("proseso hijo ejecutando: %d \n",getpid()); 
		s = num1+num2;
		printf("la suma es: %f\n", s);
		exit(0);
	}
	else{
		//sleep(5);
		printf("proceso padre ejecutado: %d\n", getpid());
		r = num1-num2;
		printf("la resta es: %f\n", r);
		pid_hijo = wait(&estado);
		printf("Proceso hijo terminado!!\n", pid_hijo);
	}  
	return 0;
}
