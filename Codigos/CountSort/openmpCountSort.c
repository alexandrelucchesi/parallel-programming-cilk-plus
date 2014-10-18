#include<stdio.h>
#include<stdlib.h>
#ifdef _OPENMP
	#include<omp.h>
#endif
int main(int argc, char** argv){
	int n,i,j,cont;
	float* vector;
	float* result;
	float* tmp;
	double time;
	int thread_count;
//It's necessary the number of threads
        if(argc==1){
                printf("One parameter is required.\n");
                return(0);
        }
        thread_count = strtol(argv[1],NULL,10);
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
	#pragma omp parallel for private(cont,j) num_threads(thread_count) schedule(auto)
	for(i=0;i<n;i=i+1){
		cont = 0;
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
//Time after the sorting
	time = omp_get_wtime() - time;
//Transfer result pointer for vector
	tmp = vector;
	vector = result;
	result = tmp;
//Print the sorted vector
//	for(i=0;i<n;i++){
//		printf("%.2f ",vector[i]);	
//	}
//Print time
	printf("\n%lf\n",time);
	free(vector);
	free(result);
	return(0);
}
