#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util/logger.h"
#include "util/input_output.h"
#include "util/thread_operacoes.h"
#include "portais/portal.h"
#include "buffers/buffer.h"

// Protótipos das funções dos buffers que serão chamadas
int executar_buffer_v1(int argc, char** argv);
int executar_buffer_v2(int argc, char** argv);
int executar_buffer_v3(int argc, char** argv);


//isso pega a variavel global de input_output, na funcao entradaDados
extern int QNT_THREADS;
extern Portal portal;


int main(int argc, char** argv){
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <modo> [opções...]\n", argv[0]);
        fprintf(stderr, "Modos disponíveis:\n");
        fprintf(stderr, "  portal\n");
        fprintf(stderr, "  buffer_v1\n");
        fprintf(stderr, "  buffer_v2\n");
        fprintf(stderr, "  buffer_v3\n");
        return 1;
    }

    // O primeiro argumento decide qual programa executar
    char* modo = argv[1];

    // Remove o primeiro argumento para passar o resto para as funções
    int novo_argc = argc - 1;
    char** novo_argv = argv + 1;

    if (strcmp(modo, "portal") == 0) {
        srand((unsigned int)time(NULL));
        DadosLidos* dadosLidos = entradaDados(argc, argv, &portal);

        int num_escritores = QNT_THREADS/2;
        int num_leitores = QNT_THREADS - num_escritores;
        int numAlunos = dadosLidos->qtd_alunos;
        pthread_t *threads_escritores = malloc(num_escritores * sizeof(pthread_t));
        pthread_t *threads_leitores = malloc(num_leitores * sizeof(pthread_t));

        /*
        * ESCREVE OS DADOS  
        */
        for(int i = 0; i < num_escritores; i++){
            ThreadArgs *args = malloc(sizeof(ThreadArgs));
            args->portal = &portal;
            args->id_thread = i + 1; 
            args->alunos_iniciais = dadosLidos->alunos;
            args->inicioAluno = (numAlunos/num_escritores) * i;
            args->fimAluno = (i == num_escritores -1) ? numAlunos : (numAlunos/num_escritores) * (i+1);


            // cria a thread
            if (pthread_create(&threads_escritores[i], NULL, thread_escritor, args) != 0) {
                perror("Erro ao criar thread escritora");
                free(args);
            }
        }

        /**
        LEITURA DE DADOS
        */
        for(int i = 0; i < num_leitores; i++){
            ThreadArgs *args = malloc(sizeof(ThreadArgs));
            args->portal = &portal;
            args->id_thread = num_leitores + i + 1;
            args->alunos_iniciais = dadosLidos->alunos;
            args->inicioAluno = (numAlunos/num_leitores) * i;
            args->fimAluno = (i == num_leitores -1) ? numAlunos : (numAlunos/num_leitores) * (i+1);
    

            if (pthread_create(&threads_leitores[i], NULL, thread_leitor, args) != 0) {
                perror("Erro ao criar thread leitora");
                free(args);
            }
        }

        //executa as threads
        for(int i = 0; i < num_escritores; i++){
            pthread_join(threads_escritores[i], NULL);
        }
        for(int i = 0; i < num_leitores; i++){
            pthread_join(threads_leitores[i], NULL);
        }

        //fecha o portal e libera os dados
        free(threads_escritores);
        free(threads_leitores);
    
        if (dadosLidos) {
            if (dadosLidos->alunos) {
                free(dadosLidos->alunos);
            }
            free(dadosLidos);
        }

        portal_close(&portal);

    } else if (strcmp(modo, "buffer_v1") == 0) {
        // Executa a versão com múltiplos produtores e um consumidor
        return executar_buffer_v1(novo_argc, novo_argv);
    } else if (strcmp(modo, "buffer_v2") == 0) {
        // Executa a versão com múltiplos produtores e múltiplos consumidores
        return executar_buffer_v2(novo_argc, novo_argv);
    } else if (strcmp(modo, "buffer_v3") == 0) {
        // Executa a versão sem controle de concorrência
        return executar_buffer_v3(novo_argc, novo_argv);
    } else {
        fprintf(stderr, "Modo '%s' desconhecido.\n", modo);
        return 1;
    }

    return 0;
}