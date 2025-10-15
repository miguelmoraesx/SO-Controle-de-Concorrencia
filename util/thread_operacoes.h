#ifndef TRHEAD_OPERACOES_H
#define TRHEAD_OPERACOES_H

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include "../portais/portal.h"

typedef struct {
    Portal *portal;
    int id_thread;
    
    //passa os registros pra guardar (dados iniciais)
    RegistroAluno *alunos_iniciais;
    int inicioAluno;
    int fimAluno;
}ThreadArgs;

void *thread_escritor(void *arg);
void *thread_leitor(void *arg);
#endif