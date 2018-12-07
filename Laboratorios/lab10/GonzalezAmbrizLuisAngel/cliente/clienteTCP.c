#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PUERTO 51718
#define TAM_BUFFER 200
#define DIR_IP "192.168.100.32"
char leer_mensaje[TAM_BUFFER];

void limpiar_cadena();
int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in direccion_servidor;
	
/*	
 *	AF_INET - Protocolo de internet IPV4
 *  htons - El ordenamiento de bytes en la red es siempre big-endian, por lo que
 *  en arquitecturas little-endian se deben revertir los bytes
 */	
	memset (&direccion_servidor, 0, sizeof (direccion_servidor));
	direccion_servidor.sin_family = AF_INET;
	direccion_servidor.sin_port = htons(PUERTO);
/*	
 *	inet_pton - Convierte direcciones de texto IPv4 en forma binaria
 */	
	if( inet_pton(AF_INET, DIR_IP, &direccion_servidor.sin_addr) <= 0 )
	{
		perror("Ocurrio un error al momento de asignar la direccion IP");
		exit(1);
	}
/*
 *	Creacion de las estructuras necesarias para el manejo de un socket
 *  SOCK_STREAM - Protocolo orientado a conexión
 */
	printf("Creando Socket ....\n");
	if( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Ocurrio un problema en la creacion del socket");
		exit(1);
	}
/*
 *	Inicia el establecimiento de una conexion mediante una apertura
 *	activa con el servidor
 *  connect - ES BLOQUEANTE
 */
	printf ("Estableciendo conexion ...\n");
	if( connect(sockfd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor) ) < 0) 
	{
		perror ("Ocurrio un problema al establecer la conexion");
		exit(1);
	}
/*
 *	Inicia la transferencia de datos entre cliente y servidor
 */
	limpiar_cadena();
	printf ("Recibiendo datos de servidor ...\n");
	if (read (sockfd, &leer_mensaje, TAM_BUFFER) < 0)
	{	
		perror ("Ocurrio algun problema al recibir datos del cliente");
		exit(1);
	}
	printf ("\tEl servidor envio latitud   ---->>:   %s\n", leer_mensaje);
	
	if( write(sockfd, "ok", 3) < 0 )
	{
		perror("Ocurrio un problema en el envio de un mensaje al cliente");
		exit(1);
	}

	limpiar_cadena();
	
	if (read (sockfd, &leer_mensaje, TAM_BUFFER) < 0)
	{	
		perror ("Ocurrio algun problema al recibir datos del cliente");
		exit(1);
	}
	printf ("\tEl servidor envio  longitud ---->>:   %s\n", leer_mensaje);
	
	if( write(sockfd, "ok", 3) < 0 )
	{
		perror("Ocurrio un problema en el envio de un mensaje al cliente");
		exit(1);
	}
	
	limpiar_cadena();

	if (read (sockfd, &leer_mensaje, TAM_BUFFER) < 0)
	{	
		perror ("Ocurrio algun problema al recibir datos del cliente");
		exit(1);
	}
	printf ("\tEl servidor envio  la hora  ---->>:   %s \n", leer_mensaje);
	if( write(sockfd, "ok", 3) < 0 )
	{
		perror("Ocurrio un problema en el envio de un mensaje al cliente");
		exit(1);
	}
	
	
	printf ("Cerrando la aplicacion cliente\n");
/*
 *	Cierre de la conexion
 */
	close(sockfd);

	return 0;
}
	



void limpiar_cadena()
{
	register int i=0;
	for (i = 0; i < TAM_BUFFER; i++)
	{
		leer_mensaje[i]='\0';
	}

}
