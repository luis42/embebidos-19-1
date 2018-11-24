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

void recibe(int cd)
{
    
    printf("\n Conexion establecida.. enviando datos..\n");   
    int i=0;
    while(i<3)
    {
    char nombre[100];
    memset(nombre,0,sizeof (nombre));
    int recibidos=0;
    char ta[100];
    memset(ta,0,sizeof(ta));
    
    recibidos=read(cd,ta,sizeof(ta));
    printf("\n-> %s\n",ta);
    if(recibidos<0)
    {
         perror("No se pudo leer el tamanio del archivo..\n");
         exit(EXIT_FAILURE);
    }//if
    printf("\n");
    i++;
  }


    
}

void mandar(int cd,char *imagen)
{


  


      
  char *arch1=imagen;
  char buffer[100];
  /*********/
  //char* local_file = "/foo/bar/baz.txt";


  char* ts2 = strdup(arch1);


  char* nombre = basename(ts2);
  /********/
  memset((char *)&buffer,0,sizeof(buffer));
  
  FILE *f = fopen(arch1, "r+");
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
            
            //printf("Se envio cadena de %d bytes\n",enviados);
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
      fclose(f);
      printf("Archivo enviado.. \n");
      
      
          

}