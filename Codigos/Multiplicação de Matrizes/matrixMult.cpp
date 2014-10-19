#include<stdio.h>
#include<stdlib.h>
#include<cilk/cilk.h>
#include<omp.h>
#include<time.h>

#define MAX 10000

//#define cilk_spawn
//#define cilk_sync
//#define cilk_for for

int main(int argc, char** argv){
	double timeZero, serialTime, cilkTime, speedup;
	int *mA, *mB, *mCcilk, *mCserial;
	int n;
	int i,j,k,in,jn;
        if(argc!=2){
		printf("É necessário um argumento para o tamanho das matrizes.\n");
                return(0);
        }

	n = strtol(argv[1],NULL,10);	

	srand((unsigned)time(NULL));
	printf("Matriz A: %d x %d\nMatriz B: %d x %d\n",n,n,n,n);

//Alocando o espaço da galera
	mA = (int *) calloc(n*n, sizeof(int));
	mB = (int *) calloc(n*n, sizeof(int));
	mCcilk = (int *) calloc(n*n, sizeof(int));
	mCserial = (int *) calloc(n*n, sizeof(int));

	if((!mA)&&(!mB)&&(!mCcilk)&&(!mCserial)){
		printf("Memória insuficiente");
		return(0);
	}

//Populando as matrizes

	cilk_for(i=0;i<n*n;i++){
		mA[i] = rand() % MAX ;
		mB[i] = rand() % MAX ;
		
	}


//MultiplicaÇão para humanos:
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

//Multipilcação utilizando cilkplus
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
 * Multiplicação utilizando notação de array... Não funciona com alocação dinâmica!!!
 * Nesse caso devem ser criados as matrizes(eu apaguei essas linhas) bidimensionais

	for(p=0; p<i; p++){
		cilk_for(q=0; q<n; q++){
			rCilk[p][q] = __sec_reduce_add(matrizA[p][:] * matrizB[:][q]);
		}
	}
	cilkTime = omp_get_wtime() - timeZero;
*/

	speedup = serialTime/cilkTime;

	printf("Tempo do algoritmo serial: %.2lf\n",serialTime);
	printf("Tempo do algoritmo cilk:   %.2lf\n",cilkTime);
	printf("Teve speedup???:           %.2lf\n",speedup);

	if(n<50){
                printf("Matriz A:\n");
                for(i=0;i<n;i++){
                        for(j=0;j<n;j++){
                                printf("%d ",mA[i*n+j]);
                        }
                        printf("\n");
                }
                printf("\nMatriz B\n");
                for(i=0;i<n;i++){
                        for(j=0;j<n;j++){
                                printf("%d ",mB[i*n+j]);
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

	}
	
	free(mA);
	free(mB);
	free(mCcilk);
	free(mCserial);

	return(0);
}
