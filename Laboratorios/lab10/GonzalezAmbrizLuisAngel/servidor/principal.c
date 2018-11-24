#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <ctype.h>
#include <sys/wait.h>


#include "defs.h"
#include "red.h"


void ISRsw ( int sig );
void error(const char *msg);


int main()
{
  
	if(signal(SIGUSR1, ISRsw)== SIG_ERR)
   	{
   		perror("Error en la ISRsw\n");
   		exit(EXIT_FAILURE);
   	}


 
 	pid_t  pid;
	int sockfd, cd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = 51717;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);


  for(;EVER;)
{
printf("--->Inicializando servidor \n");

  cd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
  
  if (cd < 0) {
      printf("No fue posible aceptar la conexion\n");
      close(cd);
      exit(EXIT_FAILURE);
  }//if   
  //recibir();
  //mandar(cd,f);
  pid = fork();
   	if(!pid)
   	{
  	mandar(cd);
  	close(cd);

  	kill ( getppid(),SIGUSR1);
   	
   	exit(0);
  	
  }
  	

}
	close(sockfd);
	return 0;
}


void ISRsw ( int sig )
{
	int estado;
	pid_t pid;
	if( sig== SIGUSR1)  //señal enviada desde el hijo..... 
	{
     wait ( & estado);
     printf("USR1 recibida por el proceso %d\n", pid);
	}
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


