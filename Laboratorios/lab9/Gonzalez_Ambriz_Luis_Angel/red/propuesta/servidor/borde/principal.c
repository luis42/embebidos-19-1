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


#include "datos.h"
#include "hilo.h"
#include "defs.h"
#include "red.h"
char *nombre_imagen_recibida;
unsigned char *imagenRGB,*imagenGray,*imagenFiltro;
uint32_t width;
uint32_t height;
int main(){
  int sd;
  struct sockaddr_in serverADDRESS;
  //struct hostent *hostINFO;
  FILE *f;
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0)
  {
     printf("No fue posible crear el socket\n");
     exit(EXIT_FAILURE);
  }//if
    //int op= setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v));
  serverADDRESS.sin_family = AF_INET;
  serverADDRESS.sin_addr.s_addr = htonl(INADDR_ANY);
  serverADDRESS.sin_port = htons(PUERTO);
  if (bind(sd, (struct sockaddr *) &serverADDRESS, sizeof(serverADDRESS)) < 0)
  {
      printf("Cannot bind socket\n");
      close(sd);
      exit(EXIT_FAILURE);
  }

  listen(sd, 5);
  socklen_t clientADDRESSLENGTH;
  struct sockaddr_in clientADDRESS;
  int cd;
  clientADDRESSLENGTH = sizeof(clientADDRESS);
  cd = accept(sd, (struct sockaddr *) &clientADDRESS, &clientADDRESSLENGTH);
  if (cd < 0) {
      printf("No fue posible aceptar la conexion\n");
      close(sd);
      exit(EXIT_FAILURE);
  }//if   
  //recibir();
  //mandar(cd,f);
  f = fdopen(cd,"w+");
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

  	close(sd);
  	close(cd);

	return 0;
}

