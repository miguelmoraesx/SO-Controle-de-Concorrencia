#ifndef TRHEAD_OPERACOES_H
#define TRHEAD_OPERACOES_H

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include "portal.h"

typedef struct {
    Portal *portal;
    int id_thread;
    int idx_aluno;

    //esses sao especificos pras escritoras
    float nova_nota;
    int delta_faltas;
}ThreadArgs;

void *thread_escritor(void *arg);
void *thread_leitor(void *arg);
#endif