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


#include "datos.h"
#include "hilo.h"
#include "defs.h"
#include "red.h"


void ISRsw ( int sig );
void error(const char *msg);

char *nombre_imagen_recibida;
unsigned char *imagenRGB,*imagenGray,*imagenFiltro;
uint32_t width;
uint32_t height;
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
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

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

  FILE *f = fdopen(cd,"w+");
  recibir(cd,f);
  sleep(1);
  //printf("\nIMAGEND recibida%s\n", nombre_imagen_recibida);
  FILE* fichero;
    
    fichero = fopen("fichero.txt", "w");
    fputs(nombre_imagen_recibida, fichero);
    
    fclose(fichero);









	int nhs[NUM_HILOS], *res;
	register int nh;
	//creas 4 hilos por que estan asi en el def
	pthread_t tids[NUM_HILOS];

	bmpInfoHeader info;
	

	imagenRGB = abrirBMP(nombre_imagen_recibida,&info);
	width=info.width;
	height=info.height;
	printf("\t--------->%d\n",height);
	displayInfo( &info);
	imagenFiltro=reservarMemoria(info.width,info.height);
	imagenGray=reservarMemoria(info.width,info.height);
	


	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, RGBtoGray, (void *)&nhs[nh]);
	}

	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **)&res);
		printf("---->  Hilo %d terminado\n", *res);
	}


	printf("Si hago RGBtoGray\n");
	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, filtroImagen, (void *)&nhs[nh]);
	}

	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **)&res);
		printf("---->  Hilo %d terminado\n", *res);
	}
	printf("Si hago filtroImagen\n");


	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		nhs[nh] = nh;
		//pthread_create(&tids[nh], NULL, funHilo, (void *)&nhs[nh]);
		pthread_create(&tids[nh], NULL, GraytoRGB, (void *)&nhs[nh]);
	}

	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **)&res);
		printf("---->  Hilo %d terminado\n", *res);
	}

	sleep(1);
	FILE *archivo;
 	
 	char caracteres[200];
 	
 	archivo = fopen("fichero.txt","r");
 	
 	if (archivo == NULL)
 		exit(1);
 	else
        {
	 	    printf("\nEl contenido del archivo de prueba es \n\n");
	 	    while (feof(archivo) == 0)
	 	    {
		 		fgets(caracteres,200,archivo);
		 		printf("%s",caracteres);
	 	    }
        }
      fclose(archivo);
	printf("---->  Guardando\n");
	char bordeq[200]="borde_";  
  	strcat(bordeq,caracteres);
  	printf("Imagene es --->%s\n",bordeq);
	guardarBMP(bordeq, &info , imagenRGB);
	f = fdopen(cd,"w+");
	nombre_imagen_recibida=bordeq;
  	mandar(cd,f);
	free(imagenRGB);
	free(imagenGray); 
	free(imagenFiltro);

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


