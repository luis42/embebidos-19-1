/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void recibe(int cd,FILE *f);
void mandar(int cd,FILE *f);
void error(const char *msg);



int main(int argc, char *argv[])
{
    int sockfd, cd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    cd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
    
    if (cd < 0) 
        error("ERROR on accept");
    bzero(buffer,256);
    FILE *f = fdopen(cd,"w+");
    recibe(cd,f);
//    n = read(cd,buffer,255);
//    write(cd,"1",18);

    if (n < 0)
        error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);




    if (n < 0)
        error("ERROR writing to socket");
    close(cd);
    close(sockfd);
    return 0; 
}





void recibe(int cd,FILE *f)
{
    
    printf("\n Conexion establecida.. enviando datos..\n");   
    char nombre[100];
    memset(nombre,0,sizeof (nombre));
    int recibidos=0;
    char ta[50];
    memset(ta,0,sizeof(ta));
    recibidos=read(cd,ta,sizeof(ta));
    printf("ta vale : %s\n",ta);
    if(recibidos<0)
    {
         perror("No se pudo leer el tamanio del archivo..");
         exit(EXIT_FAILURE);
    }//if
    int ttt = strlen(ta);
    printf("Se recibio cadena de %d bytes  %d\n",recibidos,ttt);
    char *tok=strtok(ta,";");
    char *tok1=strtok(NULL,";");
    printf("\nt1: %s\n",tok);
    printf("t1: %s\n",tok1);
    long tam=atol(tok1);
          // printf("\nTamanio del archivo: %d,  %s \n",taa,tok);
    printf("Recibiendo archivo...\n");
    int porcentaje=0;
    long a=0;
    char buffer[100];
    char *ok="ok";
    write(cd,ok,strlen(ok)+1);
    f = fopen(tok, "w+");
    printf("entro %d\n",cd);
    if (f)//rt
    { 

        while(a<tam)
        {
            memset((char *)&buffer,0,sizeof(buffer));
            int ll=read(cd,buffer,sizeof(buffer));
            if(ll<0)
            {
               perror("No se pudieron leer los datos del archivo..");
               exit(EXIT_FAILURE);
            }//if
            a = a + ll;
            fwrite(buffer,1,ll,f);
            fflush(f);
            porcentaje =(int)((a*100)/tam);
            printf("recibidos: %ld   tam: %ld  porcentaje: %d\r",a,tam,porcentaje);

        }//while
        
        
    }//fopen
}









void error(const char *msg)
{
    perror(msg);
    exit(1);
}