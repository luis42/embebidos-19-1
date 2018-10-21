#include <stdio.h>
#include <stdlib.h>
// Ejemplo aprenderaprogramar.com
int main() {
    FILE* fichero;
    char *uno="Espero se pueda esta mierda";
    fichero = fopen("cursoAF1.txt", "w");
    fputs(uno, fichero);
    
    fclose(fichero);
    printf("Proceso completado");

    FILE *archivo;
 	
 	char caracteres[200];
 	
 	archivo = fopen("cursoAF1.txt","r");
 	
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


    
    return 0;
}