#ifndef PORTAL_H
#define PORTAL_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../util/logger.h"

/* Registro do aluno */
typedef struct {
    int   id_aluno;
    float nota;
    int   faltas;
} RegistroAluno;

/* Portal com controle de concorrência (prioridade para escritores) */
typedef struct {
    RegistroAluno *alunos;
    int qtd_alunos;

    // Controle dos leitores
    int leitores_ativos;
    pthread_mutex_t mtx_leitores;

    // Semáforos do padrão "writers-first"
    sem_t turnstile;   // catraca: escritor segura para impedir novos leitores
    sem_t recurso;     // exclusão mútua do "diário" (1 escritor OU N leitores)

    Logger *logger;
} Portal;

int  portal_init(Portal *p, RegistroAluno *alunos, int qtd, Logger *lg);
void portal_close(Portal *p);

void portal_ler(Portal *p, int id_thread, int idx_aluno);
void portal_escrever(Portal *p, int id_thread, int idx_aluno, float nova_nota, int delta_faltas);

#endif
