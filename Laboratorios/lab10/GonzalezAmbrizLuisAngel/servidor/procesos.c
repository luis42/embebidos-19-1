
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

extern char *latitud;
extern char *longitud;
extern char *hora;

char validacion_de_datos(char *arreglo_dato)
{

	int inicio=strlen(arreglo_dato);
	char validacion_dato[15];
	strcpy(validacion_dato, "$GPGLL");
	char validacion_a_enviar=' ';
	
	int bandera=0;
	for (int i = 0; i < inicio; i++)
	{
		
		
		if(arreglo_dato[i] == ',' && bandera == 0)
		{	
			
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
	
	
	int bandera=0;
	if(inicio>50)
	{
		
	for (i = 0; i < inicio; i++)
	{
		
		if(arreglo_dato[i] == ',')
		{	
			
			
			bandera++;
		 
		}
		else if(bandera==1)
		{

			//printf("\n");
			char grados[2]="°";
			char minutos[3]="min";
			char segundo[3]="seg";
			char hrs[3]="hrs";
			char espacio=' ';
			char punto='.';
			int bandera_control=0;
			int contador_dato1=0;
			for (; i < inicio; i++)
			{

				if(arreglo_dato[i] == ',')
				{
					
					//printf("\n");

				}
				else if(bandera_control == 0)
				{

					latitud[contador_dato1]=arreglo_dato[i];
					latitud[++contador_dato1]=arreglo_dato[++i];
					latitud[++contador_dato1]=espacio;
					latitud[++contador_dato1]=grados[0];
					latitud[++contador_dato1]=grados[1];
					latitud[++contador_dato1]=espacio;
					latitud[++contador_dato1]=arreglo_dato[++i];
					latitud[++contador_dato1]=arreglo_dato[++i];
					latitud[++contador_dato1]=espacio;
					latitud[++contador_dato1]=minutos[0];
					latitud[++contador_dato1]=minutos[1];
					latitud[++contador_dato1]=minutos[2];
					latitud[++contador_dato1]=espacio;
					i++;
					latitud[++contador_dato1]=arreglo_dato[++i];
					latitud[++contador_dato1]=arreglo_dato[++i];
					latitud[++contador_dato1]=punto;
					latitud[++contador_dato1]=arreglo_dato[++i];
					latitud[++contador_dato1]=arreglo_dato[++i];
					latitud[++contador_dato1]=arreglo_dato[++i];
					latitud[++contador_dato1]=espacio;
					latitud[++contador_dato1]=segundo[0];
					latitud[++contador_dato1]=segundo[1];
					latitud[++contador_dato1]=segundo[2];
					latitud[++contador_dato1]=espacio;
					latitud[++contador_dato1]='N';
					i++;
					i++;
					//printf("\t\t\t--------->%s\n",latitud);
					bandera_control=1;
					//strcat(latitud,latitud);





				}
				else if(bandera_control == 1)
				{
					
					contador_dato1=0;
					longitud[contador_dato1]=arreglo_dato[i];
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=espacio;
					longitud[++contador_dato1]=grados[0];
					longitud[++contador_dato1]=grados[1];
					longitud[++contador_dato1]=espacio;
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=espacio;
					longitud[++contador_dato1]=minutos[0];
					longitud[++contador_dato1]=minutos[1];
					longitud[++contador_dato1]=minutos[2];
					longitud[++contador_dato1]=espacio;
					i++;
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=punto;
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=arreglo_dato[++i];
					longitud[++contador_dato1]=espacio;
					longitud[++contador_dato1]=segundo[0];
					longitud[++contador_dato1]=segundo[1];
					longitud[++contador_dato1]=segundo[2];
					longitud[++contador_dato1]=espacio;
					longitud[++contador_dato1]='W';
					i++;
					i++;
					
					//printf("\t\t\t--------->%s\n",longitud);
					bandera_control=2;
					//strcat(latitud,latitud);





				}
				else if(bandera_control == 2)
				{

					char conversion[2];
					contador_dato1=0;
					hora[contador_dato1]=arreglo_dato[i];
					hora[++contador_dato1]=arreglo_dato[++i];
					hora[++contador_dato1]=espacio;
					hora[++contador_dato1]=hrs[0];
					hora[++contador_dato1]=hrs[1];
					hora[++contador_dato1]=hrs[2];
					hora[++contador_dato1]=espacio;
					hora[++contador_dato1]=arreglo_dato[++i];
					hora[++contador_dato1]=arreglo_dato[++i];
					hora[++contador_dato1]=espacio;
					hora[++contador_dato1]=minutos[0];
					hora[++contador_dato1]=minutos[1];
					hora[++contador_dato1]=minutos[2];
					hora[++contador_dato1]=espacio;
					hora[++contador_dato1]=arreglo_dato[++i];
					hora[++contador_dato1]=arreglo_dato[++i];
					hora[++contador_dato1]=espacio;
					hora[++contador_dato1]=segundo[0];
					hora[++contador_dato1]=segundo[1];
					hora[++contador_dato1]=segundo[2];
					hora[++contador_dato1]=espacio;
					conversion[0]=hora[0];
					conversion[1]=hora[1];
					int num = atoi(conversion);
					if(num>=0 && num<6)
					{
						
						num=(24+num)-6;
						sprintf(conversion, "%d", num);
						hora[0]=conversion[0];
						hora[1]=conversion[1];


					}
					else if(num >5 && num < 16)
					{
						num=(0+num)-6;
						sprintf(conversion, "%d", num);
						hora[1]=conversion[0];
						hora[0]='0';

					}
					else{
						num=(0+num)-6;
						sprintf(conversion, "%d", num);
						hora[0]=conversion[0];
						hora[1]=conversion[1];
					}
					
					

					
					//printf("\t\t\t--------->%s\n",hora);
					break;
					//strcat(latitud,latitud);





				}

				else printf("%c\n",arreglo_dato[i]);
				
				
			}
			break;


			
		}

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

