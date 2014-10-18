#include<stdio.h>
#include<stdlib.h>
#include<cilk/cilk.h>
#include<omp.h>
//#define cilk_for for
int main(int argc, char** argv){
	int n,i,j,cont;
	float* vector;
	float* result;
	float* tmp;
	double time;

//Uma porcaria o tempo de execução no PC testado

//Size of the vector
	scanf("%d",&n);
//Allocating memory for vectors
	vector = (float *)malloc(n*sizeof(float));
	if(!vector){
		printf("Insufficient memory!");
		return(0);
	}
	result = (float *)calloc(n,sizeof(float));
        if(!result){
                printf("Insufficient memory!");
                return(0);
        }
//Data input
	for(i=0;i<n;i=i+1){
		scanf("%f",vector+i);
	}
//Start time counter befor the sorting step
	time = omp_get_wtime();
//Parallel sorting
	cilk_for(int i=0;i<n;i=i+1){
		int cont = 0;
		for(j=0;j<n;j=j+1){
			if(vector[i] > vector[j]){
				cont = cont + 1;
			}
			else if ( (vector[i] == vector[j]) && (i > j) ){
				cont = cont + 1;
			}
		}
		result[cont] = vector[i];
	}
//Print the sorted vector
//	for(i=0;i<n;i++){
//		printf("%.2f ",result[i]);	
//	}
//Print time
	printf("\n%lf\n",omp_get_wtime()-time);
	free(vector);
	free(result);
	return(0);
}
