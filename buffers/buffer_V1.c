#include "buffer.h"

// buffer circular
int buffer[TAM_BUFFER];
int pos_inserir = 0;
int pos_remover = 0;

// controle de sincronização
sem_t vagas;  // controla quantas posições estão livres
sem_t itens;  // controla quantos itens existem pra consumir
sem_t mutex;  // exclusão mútua (trava o acesso ao buffer)

void imprimir_buffer() {
    printf("[buffer] ");
    for (int i = 0; i < TAM_BUFFER; i++) printf("%d ", buffer[i]);
    printf("\n");
}

// função das threads produtoras
void* produtor(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % 100;  // gera algo aleatório

        sem_wait(&vagas);  // espera até ter espaço
        sem_wait(&mutex);  // entra na seção crítica

        buffer[pos_inserir] = item;
        printf("Produtor %d → produziu %d (pos=%d)\n", id, item, pos_inserir);
        pos_inserir = (pos_inserir + 1) % TAM_BUFFER;
        imprimir_buffer();

        sem_post(&mutex);  // libera acesso
        sem_post(&itens);  // sinaliza que tem item novo

        usleep((rand()%300 + 100) * 1000);  // pausa aleatória
    }
}

// consumidor único
void* consumidor(void* arg) {
    (void)arg;
    while (1) {
        sem_wait(&itens);  // espera existir item
        sem_wait(&mutex);  // entra na seção crítica

        int item = buffer[pos_remover];
        printf("Consumidor → consumiu %d (pos=%d)\n", item, pos_remover);
        buffer[pos_remover] = -1;
        pos_remover = (pos_remover + 1) % TAM_BUFFER;
        imprimir_buffer();

        sem_post(&mutex);
        sem_post(&vagas);

        usleep((rand()%500 + 200) * 1000);
    }
}

/** muda a main para ficar no arquivo main.c depois */

int main(int argc, char** argv) {
    int produtores = 3; // padrão
    if (argc >= 2) produtores = atoi(argv[1]);
    if (produtores <= 0) produtores = 3;

    srand((unsigned) time(NULL));

    for (int i = 0; i < TAM_BUFFER; i++) buffer[i] = -1;

    // inicia semáforos
    sem_init(&vagas, 0, TAM_BUFFER);
    sem_init(&itens, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t th_prod[produtores], th_cons;
    int idP[produtores], idC = 1;

    // cria produtores
    for (int i = 0; i < produtores; i++) {
        idP[i] = i + 1;
        pthread_create(&th_prod[i], NULL, produtor, &idP[i]);
    }

    // cria consumidor único
    pthread_create(&th_cons, NULL, consumidor, &idC);

    // aguarda (mantém rodando)
    for (int i = 0; i < produtores; i++) pthread_join(th_prod[i], NULL);
    pthread_join(th_cons, NULL);
    return 0;
}
