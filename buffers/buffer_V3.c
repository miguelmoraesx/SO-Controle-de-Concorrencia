#include "buffer.h"

// buffer e posições
int buffer[TAM_BUFFER];
int pos_inserir = 0;
int pos_remover = 0;

// semáforos? nenhum! aqui é o caos
void imprimir_buffer() {
    printf("[buffer] ");
    for (int i = 0; i < TAM_BUFFER; i++) printf("%d ", buffer[i]);
    printf("\n");
}

// produtor sem controle
void* produtor(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % 100;

        // sem espera, sem trava: todos mexem ao mesmo tempo
        buffer[pos_inserir] = item;
        printf("[SEM CTRL] Produtor %d → %d em %d\n", id, item, pos_inserir);
        pos_inserir = (pos_inserir + 1) % TAM_BUFFER;
        imprimir_buffer();

        usleep((rand()%200 + 50) * 1000);
    }
}

// consumidor sem controle
void* consumidor(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = buffer[pos_remover];
        printf("[SEM CTRL] Consumidor %d ← %d de %d\n", id, item, pos_remover);
        buffer[pos_remover] = -1;
        pos_remover = (pos_remover + 1) % TAM_BUFFER;
        imprimir_buffer();

        usleep((rand()%300 + 80) * 1000);
    }
}

/** muda a main para ficar no arquivo main.c depois */
int main(int argc, char** argv) {
    int produtores = 3, consumidores = 2;
    if (argc >= 2) produtores  = atoi(argv[1]);
    if (argc >= 3) consumidores = atoi(argv[2]);
    if (produtores <= 0) produtores = 3;
    if (consumidores <= 0) consumidores = 2;

    srand((unsigned) time(NULL));
    for (int i = 0; i < TAM_BUFFER; i++) buffer[i] = -1;

    pthread_t th_prod[produtores], th_cons[consumidores];
    int idP[produtores], idC[consumidores];

    for (int i = 0; i < produtores; i++) {
        idP[i] = i + 1;
        pthread_create(&th_prod[i], NULL, produtor, &idP[i]);
    }
    for (int j = 0; j < consumidores; j++) {
        idC[j] = j + 1;
        pthread_create(&th_cons[j], NULL, consumidor, &idC[j]);
    }

    for (int i = 0; i < produtores; i++) pthread_join(th_prod[i], NULL);
    for (int j = 0; j < consumidores; j++) pthread_join(th_cons[j], NULL);
    return 0;
}
