#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TAM_BUFFER 5


void imprimir_buffer();

// função das threads produtoras
void* produtor(void* arg);

// consumidor único
void* consumidor(void* arg);
