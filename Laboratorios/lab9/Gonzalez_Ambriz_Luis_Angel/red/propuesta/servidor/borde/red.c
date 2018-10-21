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

#include "defs.h"
extern char *nombre_imagen_recibida;


void recibir(int cd,FILE *f)
{
  f = fdopen(cd,"w+");
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
    nombre_imagen_recibida=tok;
    char *tok1=strtok(NULL,";");
    printf("\nt1: %s\n",tok);
    printf("t1: %s\n",tok1);
    long tam=atol(tok1);
          // printf("\nTamanio del archivo: %d,  %s \n",taa,tok);
    printf("Recibiendo archivo...\n");
    int porcentaje=0;
    long a=0;
    char buffer[1500];
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
        sleep(1);
        
    }//fopen

}

void mandar(int cd,FILE *f)
{


  


  f= fdopen(cd,"w+");   
  
  printf("IMAGEND FINAL SE LLAMARa -->%s\n", nombre_imagen_recibida);
  char *arch1=nombre_imagen_recibida;
  
  char buffer[100];
  /*********/
  //char* local_file = "/foo/bar/baz.txt";


  char* ts2 = strdup(arch1);


  char* nombre = basename(ts2);
  /********/
  memset((char *)&buffer,0,sizeof(buffer));
  
  f = fopen(arch1, "r+");
        if (f)//rt
        {   //fread
            int prev=ftell(f);//medimos el tamaño del archivo
            fseek(f,0L,SEEK_END);
            long sz = (long)ftell(f);
            fseek(f,prev,SEEK_SET);
            printf("El archivo a leer mide %ld bytes\n",sz);

            char msj[50];
            memset(&msj,0,sizeof(msj));
            sprintf(msj,"%s;%ld",nombre,sz);
            int enviados=write(cd,msj,strlen(msj)+1);
            fflush(f);
            //printf("Presiona enter para continuar...\n");
            //getchar();
            printf("Se escribio %s \n",msj);
            
            if(enviados<0)
            {
              perror("No se pudo mandar el tamanio del archivo..");
              exit(EXIT_FAILURE);
            }
            
            printf("Se envio cadena de %d bytes\n",enviados);
            int l;
            char ok[3];
            bzero(ok, sizeof(ok));
            int n=read(cd, ok, sizeof(ok));
            printf("%d\n",n);
            long leidos=0;
            while(leidos<sz)
            {
                memset((char *)&buffer,0,sizeof(buffer));
                l=fread(buffer, 1, sizeof(buffer), f);
                leidos = leidos + l;
                int es=write(cd,buffer,l);
                
                if(es<0)
                {
                    perror("No se pudo mandar el buffer de datos");
                    close(cd);
                    exit(EXIT_FAILURE);
                }//if
                
                fflush(f);
            }//while
            sleep(1);
            
        }//if fread
      
      printf("Archivo enviado.. \n");
      
      
          

}