#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TAM_BUFFER 8

// buffer circular e posições
int buffer[TAM_BUFFER];
int pos_inserir = 0;
int pos_remover = 0;

// semáforos
sem_t vagas;   // controla espaço disponível
sem_t itens;   // controla itens prontos
sem_t mutex;   // garante acesso exclusivo

void imprimir_buffer() {
    printf("[buffer] ");
    for (int i = 0; i < TAM_BUFFER; i++) printf("%d ", buffer[i]);
    printf("\n");
}

// produtor
void* produtor(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % 100;

        sem_wait(&vagas);  // espera vaga
        sem_wait(&mutex);  // entra na região crítica

        buffer[pos_inserir] = item;
        printf("Produtor %d → colocou %d em %d\n", id, item, pos_inserir);
        pos_inserir = (pos_inserir + 1) % TAM_BUFFER;
        imprimir_buffer();

        sem_post(&mutex);  // libera acesso
        sem_post(&itens);  // avisa que tem item novo

        usleep((rand()%250 + 80) * 1000);
    }
}

// consumidor
void* consumidor(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&itens);  // espera ter item
        sem_wait(&mutex);  // entra na região crítica

        int item = buffer[pos_remover];
        printf("Consumidor %d ← pegou %d de %d\n", id, item, pos_remover);
        buffer[pos_remover] = -1;
        pos_remover = (pos_remover + 1) % TAM_BUFFER;
        imprimir_buffer();

        sem_post(&mutex);
        sem_post(&vagas);

        usleep((rand()%400 + 120) * 1000);
    }
}

int main(int argc, char** argv) {
    int produtores = 3, consumidores = 2;
    if (argc >= 2) produtores  = atoi(argv[1]);
    if (argc >= 3) consumidores = atoi(argv[2]);
    if (produtores <= 0) produtores = 3;
    if (consumidores <= 0) consumidores = 2;

    srand((unsigned) time(NULL));
    for (int i = 0; i < TAM_BUFFER; i++) buffer[i] = -1;

    sem_init(&vagas, 0, TAM_BUFFER);
    sem_init(&itens, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t th_prod[produtores], th_cons[consumidores];
    int idP[produtores], idC[consumidores];

    // cria produtores
    for (int i = 0; i < produtores; i++) {
        idP[i] = i + 1;
        pthread_create(&th_prod[i], NULL, produtor, &idP[i]);
    }

    // cria consumidores
    for (int j = 0; j < consumidores; j++) {
        idC[j] = j + 1;
        pthread_create(&th_cons[j], NULL, consumidor, &idC[j]);
    }

    // mantém ativo
    for (int i = 0; i < produtores; i++) pthread_join(th_prod[i], NULL);
    for (int j = 0; j < consumidores; j++) pthread_join(th_cons[j], NULL);
    return 0;
}
