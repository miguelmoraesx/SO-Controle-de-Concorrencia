#include "buffer.h" 

// buffer circular e posições 
static int buffer[TAM_BUFFER];
static int pos_inserir = 0;
static int pos_remover = 0;

// controle de sincronização 
static sem_t vagas;
static sem_t itens;
static sem_t mutex;

// função de impressão 
static void imprimir_buffer() {
    printf("[buffer] ");
    for (int i = 0; i < TAM_BUFFER; i++) printf("%d ", buffer[i]);
    printf("\n");
}

// função das threads produtoras
static void* produtor(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % 100;

        sem_wait(&vagas);
        sem_wait(&mutex);

        buffer[pos_inserir] = item;
        printf("Produtor %d -> produziu %d (pos=%d)\n", id, item, pos_inserir);
        pos_inserir = (pos_inserir + 1) % TAM_BUFFER;
        imprimir_buffer();

        sem_post(&mutex);
        sem_post(&itens);

        usleep((rand() % 300 + 100) * 1000);
    }
}

// consumidor único
static void* consumidor(void* arg) {
    (void)arg;
    while (1) {
        sem_wait(&itens);
        sem_wait(&mutex);

        int item = buffer[pos_remover];
        printf("Consumidor -> consumiu %d (pos=%d)\n", item, pos_remover);
        buffer[pos_remover] = -1;
        pos_remover = (pos_remover + 1) % TAM_BUFFER;
        imprimir_buffer();

        sem_post(&mutex);
        sem_post(&vagas);

        usleep((rand() % 500 + 200) * 1000);
    }
}

// Função principal desta versão, chamada pelo main.c
int executar_buffer_v1(int argc, char** argv) {
    int produtores = 3; // padrão
    if (argc >= 2) produtores = atoi(argv[1]);
    if (produtores <= 0) produtores = 3;

    srand((unsigned)time(NULL));

    for (int i = 0; i < TAM_BUFFER; i++) buffer[i] = -1;

    sem_init(&vagas, 0, TAM_BUFFER);
    sem_init(&itens, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t th_prod[produtores], th_cons;
    int idP[produtores], idC = 1;

    for (int i = 0; i < produtores; i++) {
        idP[i] = i + 1;
        pthread_create(&th_prod[i], NULL, produtor, &idP[i]);
    }

    pthread_create(&th_cons, NULL, consumidor, &idC);

    for (int i = 0; i < produtores; i++) pthread_join(th_prod[i], NULL);
    pthread_join(th_cons, NULL);
    return 0;
}