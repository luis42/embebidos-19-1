#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define N 32
#define NUM_PROC 4

int *A,*B,*C;

int * reservarMemoria( void );

void imprimirArreglo( int datos[]);
void llenarArreglo( int datos[]);

int mayorArreglo( int datos[] );
int menorArreglo( int datos[] );
void ordenarArreglo( int datos[] );
int promedioArreglo( int datos[] );

void proceso_hijo( int np, int pipefd[] );
void proceso_padre( int pipefd[NUM_PROC][2]  );

int main(){
//	int *datos;
	int pipefd[NUM_PROC][2], edo_pipe;
	
	A=reservarMemoria();
	llenarArreglo(A);
	imprimirArreglo(A);


	B=reservarMemoria();
	llenarArreglo(B);
	imprimirArreglo(B);
	C=reservarMemoria();

		pid_t pid;

	printf("PRobando procesos\n");
	





        register int np;

        for(np = 0; np < NUM_PROC; np++ ){
        		edo_pipe = pipe(&pipefd[np][0]);

	
				if( edo_pipe == -1 ){
					perror("Error al crear el pipe\n");
					exit(EXIT_FAILURE);
				}
                pid = fork();

                if( pid == -1){
                        perror("Error al crear el proceso hijo\n");
                        exit(EXIT_FAILURE);
                }

                if( !pid ){
                        proceso_hijo( np, &pipefd[np][0] );
                }
        }

        proceso_padre( pipefd );

        imprimirArreglo(C);
		free(A);
		free(B);
		free(C);
	return 0;
}


void proceso_hijo( int np, int pipefd[] ){
	register int i;
	int tambloque= N/NUM_PROC;
	int iniBloque=np*tambloque;
	int finbloque=iniBloque+tambloque;



	printf("\t>>> Proceso hijo %d con pid %d <<<\n\n", np, getpid());
	close(pipefd[0]);
	for(i=iniBloque;i< finbloque;i++)
	{

		C[i]=A[i]*B[i];

	}
	write( pipefd[1] , C+iniBloque , sizeof(int)*tambloque );
	close(pipefd[1]);
	exit( np );
}

void proceso_padre( int pipefd[NUM_PROC][2] ){
        register int np;
        int npc,tambloque=N/NUM_PROC,iniBloque;
        pid_t pid_hijo;

        printf("Proceso padre con pid %d\n", getpid());
		
        for( np = 0; np < NUM_PROC; np++){
          close( pipefd[np][1]);
          pid_hijo = wait( &npc );
          npc=npc>>8;
          iniBloque=npc*tambloque;
          read(pipefd[np][0],C+iniBloque,sizeof(int)*tambloque);
          printf("Procose hijo %d terminado con pid %d\n",npc,pid_hijo );
        	

     	
	close( pipefd[npc][0]);    
		
	}
}

void ordenarArreglo( int datos[] ){
	register int n, m;
	int aux;

  	for ( n = 0; n < N; n++){
   		for ( m = 0; m < (N - n - 1); m++){
      			if (datos[m] > datos[m+1]){
       				aux = datos[m];
        			datos[m]   = datos[m+1];
        			datos[m+1] = aux;
      			}
    		}
  	}
}

int promedioArreglo( int datos[] ){
	register int n;
   	int suma = 0;
	int  promedio;

        for( n = 0; n < N; n++){
		suma += datos[n];
        }

	promedio = suma/N;
        return promedio;
}

int menorArreglo( int datos[] ){
        register int n;
        int menor;
        menor = datos[0];
        for( n = 0; n < N; n++){
                if( datos[n] < menor){
                        menor = datos[n];
                }
        }
        return menor;
}


int mayorArreglo( int datos[] ){
	register int n;
	int mayor;
	mayor = datos[0];

	for( n = 0; n < N; n++){
                if( datos[n] > mayor){
			mayor = datos[n];
		}
        }
	return mayor;
}

void imprimirArreglo( int datos[] ){
	register int n;
        for( n = 0; n < N; n++){
		if( !(n % 16))
			printf("\n");
        	printf("%5d ", datos[n]);
        }
	printf("\n");
}

void llenarArreglo( int datos[] ){
        register int n;
        //srand(getpid());
        for( n = 0; n < N; n++){
                datos[n] = rand() % 255;
        }
}

int * reservarMemoria( void ){
	int *mem;
	mem = (int *)malloc( sizeof(int)*N );
	if( !mem ){
		perror("Error al asignar memoria\n\n");
		exit(EXIT_FAILURE);
	}
	return mem;
}



//Dos modulos mas, ordenar (burbuja) y promedio, un proceso para cada tareita (4)
//Para el viernes entregarlo en lab
