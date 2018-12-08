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
#include <syslog.h>
#include "red.h"
#define EVER  1
#define MAX     10

void ISRsw ( int sig );
void error(const char *msg);
int config_serial ( char *, speed_t );
void imprimir(char *arreglo_dato);
char * limpiar_cadena(char *arreglo_dato);
char * limpiar_c();
void obtenerdatos(char * arreglo_dato);
char validacion_de_datos(char *arreglo_dato);
void posicion();
void demonio();



char *latitud;
char *longitud;
char *hora;


int main()
{
  


    demonio();
    pid_t  pid;
    int sockfd, cd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = 51718;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);


  for(;EVER;)
{
syslog(LOG_INFO, "inicializando Servidor \n");
 
  cd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
  
    posicion();
    sleep(1);
     
  if (cd < 0) {
      syslog(LOG_INFO, "No fue posible aceptar la conexion \n");
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

    //kill ( getppid(),SIGUSR1);
    
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
        syslog(LOG_INFO, "USR1 recibida por el proceso \n");
    }
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void posicion()
{
    latitud=limpiar_c();
    longitud=limpiar_c();
    hora=limpiar_c();

    register int i=0;
    int fd_serie;
    char dato;
    char *guardar;
    guardar=(char*)malloc(200);
    fd_serie = config_serial( "/dev/ttyACM0", B9600 );
    syslog(LOG_INFO, "Serial abierto con el descriptor : %d\n",fd_serie);

    //Leemos N datos del UART
    guardar=limpiar_cadena(guardar);
    while(1)
    {
        read ( fd_serie, &dato, 1 );
        if(dato == '\n')
        {   i=0;
            char validando=' ';
            validando=validacion_de_datos(guardar);
            //imprimir(guardar);
            if(validando == '1')
            {
                
                //imprimir(cadena);
                //printf("%s\n",guardar);
                obtenerdatos(guardar);
                syslog(LOG_INFO, "%s \n",latitud);
                //imprimir(latitud);
                syslog(LOG_INFO, "%s\n",longitud);
                //imprimir(longitud);
                syslog(LOG_INFO, "%s\n",hora);
                //imprimir(hora);
                break;
            }
            guardar=limpiar_cadena(guardar);
            
        }
        else
        {
        
        //printf("%c", dato);
        guardar[i]=dato;
        i++;
        }
    }

    close( fd_serie );

    
}




void imprimir(char *arreglo_dato)
{
    int inicio=strlen(arreglo_dato),i;
    
    for (i = 0; i < inicio; i++)
    {
        if(arreglo_dato[i] == '\0')
            break;
        else 
            syslog(LOG_INFO, "%c",arreglo_dato[i]);

    }
    
}


char * limpiar_cadena(char *arreglo_dato)
{
    register int i=0;
    int inicio=strlen(arreglo_dato);
    for (i = 0; i < inicio; i++)
    {
        arreglo_dato[i]='\0';
    }
    return arreglo_dato;

}



char * limpiar_c()
{
    register int i=0;
    char *arreglo_dato=(char*)malloc(60);
    for (i = 0; i < 60; i++)
    {
        arreglo_dato[i]='\0';
    }
    return arreglo_dato;

}


char validacion_de_datos(char *arreglo_dato)
{
    syslog(LOG_INFO, "\n%s\n"arreglo_dato);

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
                    hora[++contador_dato1]='\0';
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

                else syslog(LOG_INFO, "%c\n",arreglo_dato[i] );
                
                
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
        syslog(LOG_INFO, "Error al abrir el dispositivo tty \n");
        exit( EXIT_FAILURE );
    }
/*
 * cflag - Proporciona los indicadores de modo de control
 *  CBAUD   - Velocidad de transmision en baudios.
 *  CS8 - Especifica los bits por dato, en este caso 8
 *  CLOCAL  - Ignora las lineas de control del modem: CTS y RTS
 *  CREAD   - Habilita el receptor del UART
 * iflag - proporciona los indicadores de modo de entrada
 *  IGNPAR  - Ingnora errores de paridad, es decir, comunicación sin paridad
 * oflag - Proporciona los indicadores de modo de salida
 * lflag - Proporciona indicadores de modo local
 *  TCIOFLUSH - Elimina datos recibidos pero no leidos, como los escritos pero no transmitidos
 *  ~ICANON - Establece modo no canonico, en este modo la entrada esta disponible inmediatamente
 * cc[]  - Arreglo que define caracteres especiales de control
 *  VMIN > 0, VTIME = 0 - Bloquea la lectura hasta que el numero de bytes (1) esta disponible
 */
    newtermios.c_cflag  = CBAUD | CS8 | CLOCAL | CREAD;
    newtermios.c_iflag  = IGNPAR;
    newtermios.c_oflag  = 0;
    newtermios.c_lflag  = TCIOFLUSH | ~ICANON;
    newtermios.c_cc[VMIN]   = 1;
    newtermios.c_cc[VTIME]  = 0;

// Configura la velocidad de salida del UART
    if( cfsetospeed( &newtermios, baudios ) == -1 )
    {
        syslog(LOG_INFO, "Error al establecer velocidad de salida \n");
        exit( EXIT_FAILURE );
    }
// Configura la velocidad de entrada del UART
    if( cfsetispeed( &newtermios, baudios ) == -1 )
    {
        syslog(LOG_INFO, "Error al establecer velocidad de entrada \n");
        exit( EXIT_FAILURE );
    }
// Limpia el buffer de entrada
    if( tcflush( fd, TCIFLUSH ) == -1 )
    {
        syslog(LOG_INFO, "Error al limpiar el buffer de entrada \n");
        exit( EXIT_FAILURE );
    }
// Limpia el buffer de salida
    if( tcflush( fd, TCOFLUSH ) == -1 )
    {
        
        syslog(LOG_INFO, "Error al limpiar el buffer de salida \n");
        exit( EXIT_FAILURE );
    }
/*
 * Se establece los parametros de terminal asociados con el
 * descriptor de archivo fd utilizando la estructura termios
 * TCSANOW - Cambia los valores inmediatamente
 */
    if( tcsetattr( fd, TCSANOW, &newtermios ) == -1 )
    {
        syslog(LOG_INFO, "Error al establecer los parametros de la terminal \n");
        exit( EXIT_FAILURE );
    }
//Retorna el descriptor de archivo
    return fd;
}




void demonio()
{
	FILE *apArch;

    pid_t pid = 0;
    pid_t sid = 0;
    int cont = 0;
// Se crea el proceso hijo
    pid = fork();
    if( pid == -1 )
    {
		perror("Error al crear el primer proceso hijo\n");
		exit(EXIT_FAILURE);
    }
/*
 * Se termina Proceso padre.
 * Al finalizar el proceso padre el proceso hijo es adoptado por init. 
 * El resultado es que la shell piensa que el comando terminó con éxito, 
 * permitiendo que el proceso hijo se ejecute de manera independiente en segundo plano.
 */
    if( pid )
    {
		printf("Se termina proceso padre, PID del proceso hijo %d \n", pid);
		exit(0);
    }
/* Se restablece el modo de archivo
 * Todos los procesos tiene una máscara que indica que permisos no deben establecerse al crear nuevos archivos. 
 * Así cuando se utilizan llamadas al sistema como open() los permisos especificados se comparan con esta máscara, 
 * desactivando de manera efectiva los que en ella se indiquen.
 * La máscara —denominada umask()— es heredada de padres a hijos por los procesos, por lo que su valor por defecto 
 * será el mismo que el que tenía configurada la shell que lanzó el demonio. Esto significa que el demonio no sabe 
 * que permisos acabarán tenido los archivos que intente crear. Para evitarlo simplemente podemos autorizar todos 
 * los permisos 
 */
    umask(0);
/*
 * se inicia una nueva sesion
 * Cada proceso es miembro de un grupo y estos a su vez se reúnen en sesiones. En cada una de estas hay un proceso 
 * que hace las veces de líder, de tal forma que si muere todos los procesos de la sesión reciben una señal SIGHUP.
 * La idea es que el líder muere cuando se quiere dar la sesión por terminada, por lo que mediante SIGHUP se 
 * notifica al resto de procesos esta circunstancia para que puedan terminar ordenadamente.
 * Obviamente no estamos interesados en que el demonio termine cuando la sesión desde la que fue creado finalice, 
 * por lo que necesitamos crear nuestra propia sesión de la que dicho demonio será el líder.
 */
    sid = setsid();
    if( sid < 0 )
    {
		perror("Error al iniciar sesion");
		exit(EXIT_FAILURE);
    }
// Se realiza un segundo fork para separarnos completamente de la sesion del padre
    pid = fork( );
    if( pid == -1 )
    {
		perror("Error al crear el segundo proceso hijo\n");
		exit(EXIT_FAILURE);
    }
    if( pid )
    {
		printf("PID del segundo proceso hijo %d \n", pid);
		apArch = fopen("/run/demonio.pid", "w");
		fprintf(apArch, "%d", pid);
		fclose(apArch);

		exit(0);
    }
/* 
 * Se cambia el directorio actual por root.
 * Hasta el momento el directorio de trabajo del proceso es el mismo que el de la shell en el momento en el
 * que se ejecutó el comando. Este podría estar dentro de un punto de montaje cualquiera del sistema, por lo
 * que no tenemos garantías de que vaya a seguir estando disponible durante la ejecución del proceso.
 * Por eso es probable que prefiramos cambiar el directorio de trabajo al directorio raíz, ya que podemos
 * estar seguros de que siempre existirá
 */
    chdir("/");
/*
 * Se cierran los flujos de entrada y salida: stdin, stdout, stderr
 * Puesto que un demonio se ejecuta en segundo plano no debe estar conectado a ninguna terminal. 
 * Sin embargo esto plantea la cuestión de cómo indicar condiciones de error, advertencias u otro 
 * tipo de sucesos del programa. Algunos demonios almacenan estos mensajes en archivos específicos 
 * o en su propia base de datos de sucesos. Sin embargo en muchos sistemas existe un servicio especifico 
 * para registrar estos eventos. En lo sistemas basados en UNIX este servicio lo ofrece el demonio Syslog, 
 * al que otros procesos pueden enviar mensajes a través de la función syslog()
 */
    close( STDIN_FILENO  );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );
// Se abre un archivo log en modo de escritura.
    openlog( "demonio", LOG_NDELAY | LOG_PID, LOG_LOCAL0 );

    for( cont = 0; cont < MAX; cont++ ) 
    {
//No bloquea el cambio de contexto, Solo se duerme al proceso por algún tiempo
		sleep(1);
// Codigo del demonio ......
		syslog(LOG_INFO, "Demonio inicializado %d", cont );
    }
    closelog( );

}


