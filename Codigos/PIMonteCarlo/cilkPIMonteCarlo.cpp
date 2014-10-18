#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<cilk/cilk.h>
#include<cilk/reducer_opadd.h>
//#include<cilk/cilk_api.h>

//#define cilk_spawn
//#define cilk_sync
//#define cilk_for for

//NÃO ESTÁ GERANDO O MESMO RESULTADO NESSA DESGRAÇA 
//E O CILK É UM MILHÃO DE VEZES PIOR - CONSERTAR OU IMPLEMENTAR OUTRO!!!

int main(int argc, char** argv){
	double x;
	double y;
	long long pontos;
	int numThreads;
	long long int i;
	struct timeval ini, fim;
	long long unsigned duracao;
	unsigned id;
	
	cilk::reducer_opadd<long long int> cont;

	gettimeofday(&ini,NULL);

	if(argc!=3){
		printf("Dois parametros requeridos.\n");
		printf("Use ./program <numero de threads> <numero de pontos>\n");
		return(0);
	}
	numThreads = strtol(argv[1],NULL,10);
	pontos = strtol(argv[2],NULL,10);

//	__cilkrts_set_param("nworkers", "20");

	gettimeofday(&ini,NULL);

	cilk_for(i=0; i<pontos; i++){
                x = rand_r(&id)/(double)RAND_MAX;
                y = rand_r(&id)/(double)RAND_MAX;
		if(x*x + y*y <= 1){
			cont = cont + 1;
		}
	}
        gettimeofday(&fim,NULL);
        duracao = (long long unsigned)((fim.tv_sec*1000000 + fim.tv_usec)-(ini.tv_sec*1000000 + ini.tv_usec));
	printf("%f\n",(double)4*cont.get_value()/(double)pontos);
	printf("%lld\n",duracao);
        
	return(0);
}
