#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
//#include <sys/stat.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>

#include "datos.h"
#include "defs.h"
#include "procesos.h"






extern char *latitud;
extern char *longitud;
extern char *hora;

void mandar(int cd)
{

	int n;
	

	char buffer[100];
	sprintf( buffer, "%s \n", latitud );
	if ( (n = write( cd, buffer, strlen(buffer) ) ) < 0 )
		perror( "ERROR writing to socket");
	buffer[n] = '\0';


	int n2;
	

	char buffer2[100];


	sprintf( buffer2, "%s \n", longitud );
	if ( (n2 = write( cd, buffer2, strlen(buffer2) ) ) < 0 )
		perror( "ERROR writing to socket");
	buffer2[n] = '\0';


	int n3;
	

	char buffer3[100];
	sprintf( buffer3, "%s \n", hora );
	if ( (n3 = write( cd, buffer3, strlen(buffer3) ) ) < 0 )
		perror( "ERROR writing to socket");
	buffer3[n] = '\0';
	printf("saliendo\n");
	close(cd);
}


