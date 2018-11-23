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


#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define N  1024
#define EVER 1



int config_serial ( char *, speed_t );
void imprimir(char *);
char * limpiar_cadena(char *arreglo_dato);
char validacion_de_datos(char *arreglo_dato);
void obtenerdatos(char * arreglo_dato);


char *latitud;
char *norte;
char *longitud;
char *este;
char *hora;
int main()
{
	register int i;
	int fd_serie;
	unsigned char dato;
	unsigned short int tempL,tempH,temp;
	fd_serie = config_serial( "/dev/ttyACM0", B9600 );
	
	printf("serial abierto con descriptor: %d\n", fd_serie);
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
		 		
		 		imprimir(cadena);
		 		obtenerdatos(cadena);
		 		
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

	close( fd_serie );

	return 0;
}




void imprimir(char *arreglo_dato)
{
	int inicio=strlen(arreglo_dato),i;
	
	for (i = 0; i < inicio; i++)
	{
		printf("%c",arreglo_dato[i]);
	}
	printf("\n Finaliza lectura de datos\n");

}



char * limpiar_cadena(char *arreglo_dato)
{
	register int i=0;
	for (i = 0; i < 200; i++)
	{
		arreglo_dato[i]='\0';
	}
	return arreglo_dato;

}


char validacion_de_datos(char *arreglo_dato)
{

	int inicio=strlen(arreglo_dato),i;
	char validacion_dato[15];
	strcpy(validacion_dato, "$GPGLL");
	char validacion_a_enviar=' ';
	int dato_guardado_para_partir=0;
	int bandera=0;
	for (int i = 0; i < inicio; i++)
	{
		
		if(arreglo_dato[i] == ',' && bandera == 0)
		{	
			dato_guardado_para_partir=i;
			bandera++;
			int j=0;
			int ret;
			char datos_procesar[i];
			for(j=0; j<i; j++)
			{
				datos_procesar[j]=arreglo_dato[j];

			}
			//printf("\n\ncadenas a comparar\n");
			//printf("%s\n%s\n",validacion_dato,datos_procesar);

			//printf("\n\n");
			ret = strcmp(validacion_dato, datos_procesar);
			if(ret == 0)
			{
				//printf("ya encontrmos a $GPGLL \n");
				validacion_a_enviar='1';
			}
			else
			{
				//printf("aun no encontrmos a $GPGLL\n");
				validacion_a_enviar='0';

			}

		 
		}

	}

	return validacion_a_enviar;





}

void obtenerdatos(char * arreglo_dato)
{



	int inicio=strlen(arreglo_dato),i;
	char validacion_dato[15];
	char validacion_a_enviar=' ';
	int dato_guardado_para_partir=0;
	int bandera=0;
	for (int i = 0; i < inicio; i++)
	{
		
		if(arreglo_dato[i] == ',' && bandera == 0)
		{	
		
			bandera++;
		 
		}
		else
		{
			printf("empiezan da\n");
			break;
		}

	}

}
/** @brief: Esta funcion Configura la interfaz serie
 *  @param: dispositivo_serial, Nombre el dispositivo serial a usar: ttyUSB0, ttyUSB1, etc
 *  @param: baudios, Velocidad de comunicacion. Se usa la constante Bxxxx, donde xxxx  es la
 *          velocidad. Estan definidas en termios.h. Ejemplo: B9600, B19200..
 *  @return: fd, Descriptor del serial
 *******************************************************************************************
 */
int config_serial( char *dispositivo_serial, speed_t baudios )
{
	struct termios newtermios;
  	int fd;
/*
 * Se abre un descriptor de archivo para manejar la interfaz serie
 * O_RDWR - Se abre el descriptor para lectura y escritura
 * O_NOCTTY - El dispositivo terminal no se convertira en el terminal del proceso
 * ~O_NONBLOCK - Se hace bloqueante la lectura de datos
 */
  	fd = open( dispositivo_serial, (O_RDWR | O_NOCTTY) & ~O_NONBLOCK );
	if( fd == -1 )
	{
		printf("Error al abrir el dispositivo tty \n");
		exit( EXIT_FAILURE );
  	}
/*
 * cflag - Proporciona los indicadores de modo de control
 *	CBAUD	- Velocidad de transmision en baudios.
 * 	CS8	- Especifica los bits por dato, en este caso 8
 * 	CLOCAL 	- Ignora las lineas de control del modem: CTS y RTS
 * 	CREAD  	- Habilita el receptor del UART
 * iflag - proporciona los indicadores de modo de entrada
 * 	IGNPAR 	- Ingnora errores de paridad, es decir, comunicación sin paridad
 * oflag - Proporciona los indicadores de modo de salida
 * lflag - Proporciona indicadores de modo local
 * 	TCIOFLUSH - Elimina datos recibidos pero no leidos, como los escritos pero no transmitidos
 * 	~ICANON - Establece modo no canonico, en este modo la entrada esta disponible inmediatamente buffer se limpia cuando se llena
 			  hace un salto de linea.
 			  al desahabilitar el bufer se limpia
 * cc[]	 - Arreglo que define caracteres especiales de control
 *	VMIN > 0, VTIME = 0 - Bloquea la lectura hasta que el numero de bytes (1) esta disponible
 */
	newtermios.c_cflag 	= CBAUD | CS8 | CLOCAL | CREAD;
  	newtermios.c_iflag 	= IGNPAR;
  	newtermios.c_oflag	= 0;
  	newtermios.c_lflag 	= TCIOFLUSH | ~ICANON;
  	newtermios.c_cc[VMIN]	= 1;
  	newtermios.c_cc[VTIME]	= 0;

// Configura la velocidad de salida del UART 
  	if( cfsetospeed( &newtermios, baudios ) == -1 )
	{
		printf("Error al establecer velocidad de salida \n");
		exit( EXIT_FAILURE );
  	}
// Configura la velocidad de entrada del UART
	if( cfsetispeed( &newtermios, baudios ) == -1 )
	{
		printf("Error al establecer velocidad de entrada \n" );
		exit( EXIT_FAILURE );
	}
// Limpia el buffer de entrada
	if( tcflush( fd, TCIFLUSH ) == -1 )
	{
		printf("Error al limpiar el buffer de entrada \n" );
		exit( EXIT_FAILURE );
	}
// Limpia el buffer de salida
	if( tcflush( fd, TCOFLUSH ) == -1 )
	{
		printf("Error al limpiar el buffer de salida \n" );
		exit( EXIT_FAILURE );
	}
/*
 * Se establece los parametros de terminal asociados con el
 * descriptor de archivo fd utilizando la estructura termios
 * TCSANOW - Cambia los valores inmediatamente
 */
	if( tcsetattr( fd, TCSANOW, &newtermios ) == -1 )
	{
		printf("Error al establecer los parametros de la terminal \n" );
		exit( EXIT_FAILURE );
	}
//Retorna el descriptor de archivo
	return fd;
}

