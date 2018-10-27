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
#include "red.h"


int main(int argc, char* argv[]){
    int cd;
    char *PUERTO="51717";
    //char *PUERTO="6002";
    //char* servidor="127.0.0.1";
    char* servidor="192.168.100.32";

    
    struct addrinfo hints;
    struct addrinfo *servinfo; 
    memset(&hints, 0, sizeof(hints)); 
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    // get ready to connect
    getaddrinfo(servidor, PUERTO, &hints, &servinfo);
    cd = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);


   

    if (connect(cd,servinfo->ai_addr,servinfo->ai_addrlen)<0)
    {
        perror("error en funcion connect()\n");
    }//if
    freeaddrinfo(servinfo);
    
   
    mandar(cd,argv[1]);
    
    recibe(cd);

    

    printf("\n");
    printf("--->Archivo recibidos \n");    
    close(cd);
       
    
return 0;
}//main


