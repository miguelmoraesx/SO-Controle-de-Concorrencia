#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "input_output.h"
#include "thread_operacoes.h"

int main(int argc, char** argv){
    Portal* portal = malloc(sizeof(Portal));
    DadosLidos* dadosLidos = entradaDados(argc,argv, portal);
    int numAlunos = dadosLidos->qtd_alunos;
    pthread_t *threads = malloc(numAlunos * sizeof(pthread_t));


    for(int i = 0; i < dadosLidos->qtd_alunos; i++){
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->portal = portal;
        args->id_thread = i + 1; 
        args->idx_aluno = i;
        args->nova_nota = dadosLidos->alunos[i].nota;
        args->delta_faltas = dadosLidos->alunos[i].faltas;

        // 2. Cria a thread
        if (pthread_create(&threads[i], NULL, thread_escritor, args) != 0) {
            perror("Erro ao criar thread escritora");
            free(args);
        }
    }

    for(int i = 0; i < numAlunos; i++){
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < numAlunos; i++){
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->portal = portal;
        args->id_thread = numAlunos + i + 1;
        args->idx_aluno = i;

        if (pthread_create(&threads[i], NULL, thread_leitor, args) != 0) {
            perror("Erro ao criar thread leitora");
            free(args);
        }
    }

    for(int i = 0; i < numAlunos; i++){
        pthread_join(threads[i], NULL);
    }

    free(threads);
   
    if (dadosLidos) {
        if (dadosLidos->alunos) {
            free(dadosLidos->alunos);
        }
        free(dadosLidos);
    }

    portal_close(portal);
    return 0;
}
