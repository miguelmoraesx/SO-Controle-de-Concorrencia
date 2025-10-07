#include "thread_operacoes.h"

void *thread_escritor(void *arg){
    ThreadArgs *args = (ThreadArgs*)arg;

    portal_escrever(args->portal,
                    args->id_thread,
                    args->idx_aluno,
                    args->nova_nota,
                    args->delta_faltas);

    free(args);

    return NULL;
}
void *thread_leitor(void *arg){
ThreadArgs *args = (ThreadArgs *)arg;
    
    portal_ler(args->portal, 
                args->id_thread, 
                args->idx_aluno);
    
    free(args);
    return NULL;
}