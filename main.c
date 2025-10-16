#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util/logger.h"
#include "util/input_output.h"
#include "util/thread_operacoes.h"
#include "portais/portal.h"
#include "buffers/buffer.c"

//isso pega a variavel global de input_output, na funcao entradaDados
extern int QNT_THREADS;
extern Portal portal;


int main(int argc, char** argv){

    DadosLidos* dadosLidos = entradaDados(argc,argv, &portal);

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
    return 0;
}
