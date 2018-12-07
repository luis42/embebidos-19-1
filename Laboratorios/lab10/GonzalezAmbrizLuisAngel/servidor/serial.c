/** @brief: Este programa muestra el uso del UART
*/


/*
Guardar los datos de GPLL y analizar la cadena en algunas variables 
las cuales las va a pedir el cliente por tcp el cual siempre se va
a calcular 


https://www.gpsinformation.org/dale/nmea.htm


Se va a leer el dato de GLL en el cual el usuario va a pedir los datos 
y se van a mandar una cadena la cual se debe de analizar para que la visualice 
el usuario

-> La cadena a analizar 


$GPGLL,4916.45,N,12311.12,W,225444,A,*1D

Where:
     GLL          Geographic position, Latitude and Longitude
     4916.46,N    Latitude 49 deg. 16.45 min. North
     12311.12,W   Longitude 123 deg. 11.12 min. West
     225444       Fix taken at 22:54:44 UTC
     A            Data Active or V (void)
     *iD          checksum data
Note that, as of the 2.3 release of NMEA, there is a new field in the GLL sentence at the end just prior to the checksum. For more information on this field see here.




Polín es usar algún bit como bandera


Termios se hace la configuración del UART 




*/






#include <netdb.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "datos.h"
#include "defs.h"
#include "procesos.h"
#include "red.h"



void ISRsw ( int sig );
void error(const char *msg);
void posicion();


char *latitud;
char *longitud;
char *hora;


int main()
{
  



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
  
   	posicion();
   	 
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
  	
  	close(sockfd);

  	kill ( getppid(),SIGUSR1);
   	
   	exit(0);
  	
  }
  	
  
}
	


	close(cd);
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


void posicion()
{
	if(signal(SIGUSR1, ISRsw)== SIG_ERR)
   	{
   		perror("Error en la ISRsw\n");
   		exit(EXIT_FAILURE);
   	}
	latitud=limpiar_c();
	longitud=limpiar_c();
	hora=limpiar_c();
	int fd_serie;
	unsigned char dato;
	fd_serie = config_serial( "/dev/ttyACM0", B9600 );
	
	//printf("serial abierto con descriptor: %d\n", fd_serie);
	int contador=0;
	char *cadena;
	cadena=limpiar_cadena(cadena);
	//Leemos N datos del UART
	for( ;EVER;)
	{
		read ( fd_serie, &dato, 1 );
		
		
		//printf("%c", dato);
		
		if(dato == '\n')
		{
			char validando=' ';
		 	contador=0;
		 	printf("\n\n");
		 	//imprimir(cadena);
		 	validando=validacion_de_datos(cadena);
		 	if(validando == '1')
		 	{
		 		
		 		//imprimir(cadena);
		 		printf("%s\n",cadena);
		 		obtenerdatos(cadena);
		 		printf("%s\n", latitud);
		 		//imprimir(latitud);
		 		printf("%s\n",longitud);
		 		//imprimir(longitud);
		 		printf("%s\n", hora);
		 		//imprimir(hora);
		 		break;
		 		
		 	}
		 	cadena=limpiar_cadena(cadena);
		}
		else 
		{
			//printf("%c",dato);
			cadena[contador]=dato;
			contador++;
		}
		/*
		if(contador>9)
		{
			contador=0;
		}
		*/
	}
	sleep(1);
	close( fd_serie );
	sleep(1);
}


