#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<omp.h>
int main(int argc, char** argv){
        double x;
        double y;
        long long int cont=0;
        long long int pontos;
        int numThreads;
        long long int i;
        struct timeval ini, fim;
        long long unsigned duracao;
        unsigned id;

        if(argc!=3){
                printf("Dois parametros requeridos.\n");
                printf("Use ./program <numero de threads> <numero de pontos>\n");
                return(0);
        }
        numThreads = strtol(argv[1],NULL,10);
        pontos = strtol(argv[2],NULL,10);

        gettimeofday(&ini,NULL);
#pragma omp parallel num_threads(numThreads) private(id)
{
        id = omp_get_thread_num();
#pragma omp  for private(i,x,y) reduction(+:cont)
        for(i=0;i<pontos;i++){
                x = rand_r(&id)/(double)RAND_MAX;
                y = rand_r(&id)/(double)RAND_MAX;
                if(x*x + y*y <= 1){
                        cont++;
                }
        }
}
        gettimeofday(&fim,NULL);
        duracao = (long long unsigned)((fim.tv_sec*1000000 + fim.tv_usec)-(ini.tv_sec*1000000 + ini.tv_usec));
        printf("%f\n",(double)4*cont/(double)pontos);
        printf("%lld\n",duracao);
        return(0);
}

