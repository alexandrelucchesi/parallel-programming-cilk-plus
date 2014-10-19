#include<stdio.h>
#include<stdlib.h>
#include<cilk/cilk.h>
#include<omp.h>
#include<time.h>

#define MAX 10000

// Use when you want a serial version
//#define cilk_spawn
//#define cilk_sync
//#define cilk_for for

int main(int argc, char** argv){
	double timeZero, serialTime, cilkTime, speedup;
	int *mA, *mB, *mCcilk, *mCserial;
	int n;
	int i,j,k,in,jn;
        if(argc!=2){
		printf("One parameter is necessary: <Number of Elements>\n");
                return(0);
        }

	n = strtol(argv[1],NULL,10);	

	srand((unsigned)time(NULL));
	printf("Matriz A: %d x %d\nMatriz B: %d x %d\n",n,n,n,n);

//Allocation with dynamic memory
	mA = (int *) calloc(n*n, sizeof(int));
	mB = (int *) calloc(n*n, sizeof(int));
	mCcilk = (int *) calloc(n*n, sizeof(int));
	mCserial = (int *) calloc(n*n, sizeof(int));

	if((!mA)&&(!mB)&&(!mCcilk)&&(!mCserial)){
		printf("Insufficient memory");
		return(0);
	}

//Input values
	cilk_for(i=0;i<n*n;i++){
		mA[i] = rand() % MAX ;
		mB[i] = rand() % MAX ;
	}

//Serial version
	timeZero = omp_get_wtime();
	for(i=0;i<n;i++){
		in = i*n;
		for(j=0;j<n;j++){
			for(k=0;k<n;++k){
				jn = j*n;
				mCserial[in + k] += mA[in + j] * mB[jn + k];
			}
		}
	}
	serialTime = omp_get_wtime() - timeZero;

//With cilk plus
	timeZero = omp_get_wtime();
	cilk_for(i=0; i<n; i++){
		int in = i*n;
		for(j=0; j<n; j++){
			for(k=0; k<n; k++){
				int jn = j*n;
				mCcilk[in + k] += mA[in + j] * mB[jn + k];		
			}
		}
	}
	cilkTime = omp_get_wtime() - timeZero;

/*
 * Arry notations... Doesn't works with dynamic allocation
 * Creta a two-dimensional array to use
	for(p=0; p<i; p++){
		cilk_for(q=0; q<n; q++){
			rCilk[p][q] = __sec_reduce_add(matrizA[p][:] * matrizB[:][q]);
		}
	}
	cilkTime = omp_get_wtime() - timeZero;
*/

	speedup = serialTime/cilkTime;

	printf("Serial version: %.2lf\n",serialTime);
	printf("With Cilk:      %.2lf\n",cilkTime);
	printf("Speedup? :      %.2lf\n",speedup);

	if(n<50){
                printf("A:\n");
                for(i=0;i<n;i++){
                        for(j=0;j<n;j++){
                                printf("%d ",mA[i*n+j]);
                        }
                        printf("\n");
                }
                printf("\nB\n");
                for(i=0;i<n;i++){
                        for(j=0;j<n;j++){
                                printf("%d ",mB[i*n+j]);
                        }
                        printf("\n");
                }

		printf("\nSerial Answer\n");
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				printf("%d ",mCserial[i*n+j]);
			}
			printf("\n");
		}
                printf("\nCilk Answer\n");
                for(i=0;i<n;i++){
                        for(j=0;j<n;j++){
                                printf("%d ",mCcilk[i*n+j]);
                        }
                        printf("\n");
                }

	}
	
	free(mA);
	free(mB);
	free(mCcilk);
	free(mCserial);

	return(0);
}
