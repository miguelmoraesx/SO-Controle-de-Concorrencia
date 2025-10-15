#include "thread_operacoes.h"

void *thread_escritor(void *arg){
    ThreadArgs *args = (ThreadArgs*)arg;

    for(int i = args->inicioAluno; i < args->fimAluno; i++){
        float nova_nota = args->alunos_iniciais[i].nota + (rand()%3 -1); // -1, 0 ou +1
        int novas_faltas = args->alunos_iniciais[i].faltas + (rand()%5); //0,1,2,3 ou 4 faltas novas

        usleep(200000 + rand()%200000); // 200-400 ms
        portal_escrever(args->portal,
                    args->id_thread,
                    i,
                    nova_nota,
                    novas_faltas);
    }

    free(args);
}
void *thread_leitor(void *arg){
ThreadArgs *args = (ThreadArgs *)arg;
    
    for (int i = args->inicioAluno; i < args->fimAluno; i++) {
        usleep(200000 + rand()%200000); // 200-400 ms
        portal_ler(args->portal, 
                    args->id_thread, 
                    i);
    }
    
    free(args);
}