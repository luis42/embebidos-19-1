
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *suma(void *arg);
void *resta(void *arg);
void *multi(void *arg);
void *divi(void *arg);

//Se ponen globales para que sean compartidos entre los 4 hilos
int num1 = 52, num2 = 10;

int main(){
	int *res_sum, *res_res, *res_mul, *res_div;

	pthread_t tid_sum, tid_res, tid_mul, tid_div;

	//
	printf("Probando hilos...\n");
	pthread_create(&tid_sum, NULL, suma, NULL);
	pthread_create(&tid_res, NULL, resta, NULL);
	pthread_create(&tid_mul, NULL, multi, NULL);
	pthread_create(&tid_div, NULL, divi, NULL);

	//Pide un doble apuntador por eso se utiliza & con la variable *
	//Y hacer el cast porque pide un void**
	pthread_join(tid_sum, (void **) &res_sum);
	pthread_join(tid_res, (void **) &res_res);
	pthread_join(tid_mul, (void **) &res_mul);
	pthread_join(tid_div, (void **) &res_div);

	printf("La suma es: %d \n", *res_sum);
	printf("La resta es: %d \n", *res_res);
	printf("La multiplícación es: %d \n", *res_mul);
	printf("La división es: %d \n", *res_div);

	return 0;
}

//Se tiene que hacer un cast del resultado para regresarlo como apuntador (void *)
void *suma(void *arg){
	int *res = (int *)malloc(sizeof(int));

	*res = num1 + num2;

	pthread_exit((void *)res);
}

void *resta(void *arg){
	int *res = (int *)malloc(sizeof(int));

	*res = num1 - num2;

	pthread_exit((void *)res);
}

void *multi(void *arg){
	static int res;

	res = num1 * num2;

	pthread_exit((void *) &res);	
}

void *divi(void *arg){
	static int res;

	res = num1 / num2;

	pthread_exit((void *) &res);	
}

//Un problema que se tuvo es que al regresar la variable res, se borra al terminar la función
//porque es una variable local, por eso los resultados se pierden
//Por lo tanto se asignará memoria para conservarla
//O puede declararse como static