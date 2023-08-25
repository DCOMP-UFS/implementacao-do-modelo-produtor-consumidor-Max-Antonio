#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define SIZE 10 //tamanho máximo da fila
#define THREAD_NUM 3 //número de pares de threads (nesse caso, 3 produtoras e 3 consumidoras)

//Compile:  gcc -g -Wall -o parte2 parte2.c -lpthread -lrt
//Usage:    ./parte2

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
   printf("Thread: %d, Clock consumido: (%d, %d, %d)\n", processo, clock->p[0], clock->p[1], clock->p[2]);
}

void produzirClock(int threadId) {
    //cria o clock
    Clock *clock = (Clock*)malloc(sizeof(Clock));
    clock->p[0] = rand() % 10000;
    clock->p[1] = rand() % 10000;
    clock->p[2] = rand() % 10000;
    
    pthread_mutex_lock(&mutex_prod); //bloqueia acesso a região critica
    //regiao critica
    while (clockCont == SIZE) { //se tiver cheio espera até receber sinal que um clock foi consumido
        pthread_cond_wait(&condFull, &mutex_prod);
    }
    
    filaClock[clockCont] = *clock;
    clockCont++;
    printf("Thread: %d, Clock produzido: (%d, %d, %d)\n", threadId, clock->p[0], clock->p[1], clock->p[2]);
    
    pthread_mutex_unlock(&mutex_prod); //desbloqueia acesso a região critica
    pthread_cond_signal(&condEmpty); //envia sinal que a fila não está vazia
    
    free(clock);
}

void consumirClock(int threadId) {
    pthread_mutex_lock(&mutex_cons); //bloqueia acesso a região critica
    
    while (clockCont == 0) { //enquanto a fila estiver vazia espera até que um clock seja produzido
        pthread_cond_wait(&condEmpty, &mutex_cons);
    }
    
    Clock clock = filaClock[0];
    printClock(&clock, threadId);
    for (int i = 0; i < clockCont-1; i++) {
        filaClock[i] = filaClock[i+1];
    }
    clockCont--;
    
    pthread_mutex_unlock(&mutex_cons); //desbloqueia acesso a região critica
    pthread_cond_signal(&condFull); //envia sinal qu a fila não está cheia
    
}

void cenarioTesteCheia() {
    temp_espera_cons = 3;
    temp_espera_prod = 1;
}


void cenarioTesteVazia() {
    temp_espera_cons = 1;
    temp_espera_prod = 3;
}


void *threadProdutora(void* arg) {
    long id = (long) arg; 
    while (1){ 
        produzirClock(id);
        sleep(temp_espera_prod* (rand() %4));
   } 
   return NULL;
} 

void *threadConsumidora(void* arg) {
    long id = (long) arg; 
    while(1) {
        consumirClock(id);  
        sleep(temp_espera_cons*(rand() %4));
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex_cons, NULL);
    pthread_mutex_init(&mutex_prod, NULL);
    pthread_cond_init(&condEmpty, NULL);
    pthread_cond_init(&condFull, NULL);
    
    pthread_t thread[THREAD_NUM]; 
    
    //cenarioTesteCheia();
    cenarioTesteVazia();
    
    
    long i;
    for (i = 0; i < THREAD_NUM; i++){  
        if (pthread_create(&thread[i], NULL, &threadConsumidora, (void*) i) != 0) { //cria threads consumidoras
            perror("Failed to create the thread");
        }  
    }
    for (i = THREAD_NUM; i < THREAD_NUM*2; i++){  
        if (pthread_create(&thread[i], NULL, &threadProdutora, NULL) != 0) { //cria threads produtoras
            perror("Failed to create the thread");
        }  
    }
    
    for (i = 0; i < THREAD_NUM*2; i++){  
        if (pthread_join(thread[i], NULL) != 0) { //join das threads 
        perror("Failed to join the thread");
        }  
    }

    pthread_mutex_destroy(&mutex_cons);
    pthread_mutex_destroy(&mutex_prod);
    pthread_cond_destroy(&condEmpty);
    pthread_cond_destroy(&condFull);
    return 0;
}
