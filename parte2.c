#include <stdio.h>
#include <mpi.h>   
#define SIZE 10 //tamanho máximo da fila
#define THREAD_NUM 6 //número de threads no pool (3 produzem e 3 consomem)


typedef struct Clock { 
   int p[3];
} Clock;

Clock filaClock[SIZE];
int clockCont = 0;


pthread_mutex_t mutex_cons; //mutex para as threads consumidoras
pthread_mutex_t mutex_prod; //mutex para as threads produtoras
pthread_cond_t condFull;
pthread_cond_t condEmpty;

int temp_espera_prod; //tempo de espera das threds produtoras
int temp_espera_cons; //tempo de espera das threads consumidoras


void printClock(Clock *clock, int processo) {
   printf("Process: %d, Clock: (%d, %d, %d)\n", processo, clock->p[0], clock->p[1], clock->p[2]);
}

void produzirClock() {
    int p1, p2, p3;
    
    int p[3] = ;
}

void consumirClock() {
    
}

void cenarioTesteCheia() {
    temp_espera_cons = 2;
    temp_espera_prod = 1;
}


void cenarioTesteVazia() {
    temp_espera_cons = 1;
    temp_espera_prod = 2;
}

void *startThread(void* args);  

int main() {
    pthread_mutex_init(&mutex_cons, NULL);
    pthread_mutex_init(&mutex_prod, NULL);
    pthread_cond_init(&condEmpty, NULL);
    pthread_cond_init(&condFull, NULL);
    
    pthread_t thread[THREAD_NUM]; 
    
    cenarioTesteCheia();
    //cenarioTesteVazia();
    
    
    long i;
    for (i = 0; i < THREAD_NUM; i++){  
        if (pthread_create(&thread[i], NULL, &startThread, (void*) i) != 0) {
            perror("Failed to create the thread");
         }  
    }
    
    
    
    
    
    
    
    for (i = 0; i < THREAD_NUM; i++){  
        if (pthread_join(thread[i], NULL) != 0) {
        perror("Failed to join the thread");
        }  
    }
    
    pthread_mutex_destroy(&mutex_cons);
    pthread_mutex_destroy(&mutex_prod);
    pthread_cond_destroy(&condEmpty);
    pthread_cond_destroy(&condFull);
    return 0;
}


void *threadProdutora(void* args) {
   long id = (long) args; 
   while (1){ 
        produzirClock();
        sleep(temp_espera_prod*1000);
   }
   return NULL;
} 

void *threadConsumidora(void* args) {
    long id = (long) args; 
    while(1) {
        consumirClock();  
        sleep(temp_espera_cons*1000);
    }
    return NULL;
}
