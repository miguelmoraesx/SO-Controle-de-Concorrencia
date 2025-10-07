#include "thread_operacoes.h"

void *thread_escritor(void *arg){
    ThreadArgs *args = (ThreadArgs*)arg;

    for(int i = args->inicioAluno; i < args->fimAluno; i++){
        float nota_inicial = args->alunos_iniciais[i].nota;
        int faltas_iniciais = args->alunos_iniciais[i].faltas;

        portal_escrever(args->portal,
                    args->id_thread,
                    i,
                    nota_inicial,
                    faltas_iniciais);
    }

    free(args);

    return NULL;
}
void *thread_leitor(void *arg){
ThreadArgs *args = (ThreadArgs *)arg;
    
    for (int i = args->inicioAluno; i < args->fimAluno; i++) {
        portal_ler(args->portal, 
                    args->id_thread, 
                    i);
    }
    
    free(args);
    return NULL;
}