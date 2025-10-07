#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "input_output.h"
#include "thread_operacoes.h"

int main(int argc, char** argv){
    //isso pega a variavel global de input_output, na funcao entradaDados
    extern int QNT_THREADS;
    
    Portal* portal = malloc(sizeof(Portal));
    DadosLidos* dadosLidos = entradaDados(argc,argv, portal);
    int numAlunos = dadosLidos->qtd_alunos;
    pthread_t *threads = malloc(QNT_THREADS * sizeof(pthread_t));

    /*
    * ESCREVE OS DADOS  
    */
    for(int i = 0; i < QNT_THREADS; i++){
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->portal = portal;
        args->id_thread = i + 1; 
        args->alunos_iniciais = dadosLidos->alunos;
        args->inicioAluno = (numAlunos/QNT_THREADS) * i;
        args->fimAluno = (i == QNT_THREADS -1) ? numAlunos : (numAlunos/QNT_THREADS) * (i+1);


        // cria a thread
        if (pthread_create(&threads[i], NULL, thread_escritor, args) != 0) {
            perror("Erro ao criar thread escritora");
            free(args);
        }
    }

    for(int i = 0; i < QNT_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    /**
    LEITURA DE DADOS
    */
    for(int i = 0; i < QNT_THREADS; i++){
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->portal = portal;
        args->id_thread = numAlunos + i + 1;
        args->alunos_iniciais = dadosLidos->alunos;
        args->inicioAluno = (numAlunos/QNT_THREADS) * i;
        args->fimAluno = (i == QNT_THREADS -1) ? numAlunos : (numAlunos/QNT_THREADS) * (i+1);
 

        if (pthread_create(&threads[i], NULL, thread_leitor, args) != 0) {
            perror("Erro ao criar thread leitora");
            free(args);
        }
    }

    for(int i = 0; i < QNT_THREADS; i++){
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
