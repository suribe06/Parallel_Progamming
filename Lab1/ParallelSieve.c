/*
 ============================================================================
 Name        : OpenMP_Lab1.c
 Author      : Santiago Uribe y Nicolas Ibagon
 Version     :
 Copyright   : Your copyright notice
 Description : Laboratorio 1 Programacion Paralela
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define N 1000000000
static bool sieve[N+1];

void subdominio(int i_inicio, int i_puntos){
	long long int i;
	for(i=0; i<i_puntos; i++){
		if((i_inicio+i)%2 == 0){
			sieve[i_inicio+i] = false;
		}else{
			sieve[i_inicio+i] = true;
		}
	}
}

void sub(){
	long long int curr_thr, num_thr, i_puntos, i_inicio;
	long long int n_puntos = N+1;
	omp_set_num_threads(4);

	//***********************************************************
#pragma omp parallel default(shared) private(curr_thr, num_thr, i_puntos, i_inicio)
	{
		curr_thr = omp_get_thread_num();
		num_thr = omp_get_num_threads();
		i_puntos = n_puntos / num_thr;
		i_inicio = curr_thr * i_puntos;
		if (curr_thr == num_thr - 1)
			i_puntos = n_puntos - i_inicio;

		subdominio(i_inicio, i_puntos);
	}
	//***********************************************************
}

void eratosthenes(){
	long long int cs = N / omp_get_num_threads();
	long long int i, j;
	sub();
	sieve[1] = false;
	sieve[2] = true;

#pragma omp parallel
	{
		for (i=3; i<=N; i+=2){
			if(sieve[i]){
				#pragma omp parallel for schedule(static, cs)
				for(j=i*i; j<=N; j+=i){
					sieve[j] = false;
				}
			}
		}
	}
}

int main(){
	float t_start, t_end;
	t_start = omp_get_wtime();
	eratosthenes();
	t_end = omp_get_wtime() - t_start;
	/*for (int i=0; i<=N; i++){
		if(sieve[i]){
			printf("%d ", i);
	    }
	}*/
	printf("the time is: %f\n", t_end);
	return 0;
}
