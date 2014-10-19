#include<stdio.h>
#include<stdlib.h>
#include<cilk/cilk.h>
#include<cilk/cilk_api.h>
#include<omp.h>
#include<time.h>

#define MAX 10000

//#define cilk_spawn
//#define cilk_sync
//#define cilk_for for

// WORKS WELL!

int main(int argc, char** argv){
	double timeZero, serialTime, cilkTime, openmpTime;
	int *mA, *mCcilk, *mCserial, *mCopenmp;
	int n,t, scalar;
	int i,j,k,in,jn;
        if(argc!=3){
		printf("Two arguments: <Matrix Size> <Nr of Threads>\n");
                return(0);
        }

	n = strtol(argv[1],NULL,10);
	t = strtol(argv[2],NULL,10);

	srand((unsigned)time(NULL));

//Space allocation
	mA = (int *) calloc(n*n, sizeof(int));
	mCcilk = (int *) calloc(n*n, sizeof(int));
	mCserial = (int *) calloc(n*n, sizeof(int));
	mCopenmp = (int *) calloc(n*n, sizeof(int));

	if((!mA)&&(!mCcilk)&&(!mCserial)){
		printf("Insufficient memory!");
		return(0);
	}

//Input values
	cilk_for(i=0;i<n*n;i++){
		mA[i] = rand() % MAX ;
	}
	scalar = rand() % MAX ;
	printf("Scalar value: %d\n",scalar);

//Serial version
	timeZero = omp_get_wtime();
	for(i=0;i<n*n;i++){
		mCserial[i] = mA[i] * scalar;
	}
	serialTime = omp_get_wtime() - timeZero;

//Openmp version
        timeZero = omp_get_wtime();
#pragma omp parallel for num_threads(t)
        for(i=0;i<n*n;i++){
                mCopenmp[i] = mA[i] * scalar;
        }
        openmpTime = omp_get_wtime() - timeZero;

//How to change the number of threads in Cilk Plus
	__cilkrts_end_cilk();  
	__cilkrts_set_param("nworkers", ""+t);


//Cilk Plus version
	timeZero = omp_get_wtime();
	cilk_for(i=0; i<n*n; i++){
		mCcilk[i] = mA[i] * scalar;		
	}
	cilkTime = omp_get_wtime() - timeZero;

/*
 * Array notation... Doesn't works with dynamic allocation!!!
		rCilk[:] = matrizA[:] * scalar ;
	cilkTime = omp_get_wtime() - timeZero;
*/


	printf("Serial time:    %.5lf\n\n",serialTime);
	printf("OpenMP time:    %.5lf\n",openmpTime);
	printf("Openmp speedup: %.2lf\n\n",serialTime/openmpTime);
	printf("Cilk time:      %.5lf\n",cilkTime);
	printf("Cilk speedup:   %.2lf\n",serialTime/cilkTime);
/*
	if(n<50){
                printf("Matriz A:\n");
                for(i=0;i<n;i++){
                        for(j=0;j<n;j++){
                                printf("%d ",mA[i*n+j]);
                        }
                        printf("\n");
                }

		printf("\nResposta do Serial\n");
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				printf("%d ",mCserial[i*n+j]);
			}
			printf("\n");
		}
                printf("\nResposta do Cilk\n");
                for(i=0;i<n;i++){
                        for(j=0;j<n;j++){
                                printf("%d ",mCcilk[i*n+j]);
                        }
                        printf("\n");
                }
                printf("\nResposta do Openmp\n");
                for(i=0;i<n;i++){
                        for(j=0;j<n;j++){
                                printf("%d ",mCopenmp[i*n+j]);
                        }
                        printf("\n");
                }

	}
*/	
	free(mA);
	free(mCcilk);
	free(mCserial);
	free(mCopenmp);

	return(0);
}
