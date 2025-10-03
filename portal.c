/**
 * portal.c
 * ------------------------------------------
 * Implementação da versão 1 do Portal do Aluno
 * (leitores-escritores sem prioridade).
 */
#include "portal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Inicializa o portal */
int portal_init(Portal *p, RegistroAluno *alunos, int qtd, Logger *lg){
    p->alunos = malloc(sizeof(RegistroAluno)*qtd);
    if(!p->alunos) return 0;

    for(int i=0; i<qtd; i++)
        p->alunos[i] = alunos[i];

    p->qtd_alunos = qtd;
    p->leitores = 0;

    pthread_mutex_init(&p->mutex_leitores, NULL);
    sem_init(&p->recurso, 0, 1);
    p->logger = lg;
    return 1;
}

/* Libera recursos do portal */
void portal_close(Portal *p){
    free(p->alunos);
    pthread_mutex_destroy(&p->mutex_leitores);
    sem_destroy(&p->recurso);
}

/* Leitura de um registro */
void portal_ler(Portal *p, int idx){
    // Entrada da seção crítica
    pthread_mutex_lock(&p->mutex_leitores);
    p->leitores++;
    if(p->leitores == 1)
        sem_wait(&p->recurso); // primeiro leitor bloqueia escritores
    pthread_mutex_unlock(&p->mutex_leitores);

    // Seção crítica (leitura)
    logger_log(p->logger, "[Leitor] id=%d nota=%.1f faltas=%d",
               p->alunos[idx].id_aluno,
               p->alunos[idx].nota,
               p->alunos[idx].faltas);
    usleep(100000); // simula tempo de leitura

    // Saída da seção crítica
    pthread_mutex_lock(&p->mutex_leitores);
    p->leitores--;
    if(p->leitores == 0)
        sem_post(&p->recurso); // último leitor libera escritores
    pthread_mutex_unlock(&p->mutex_leitores);
}

/* Escrita de um registro */
void portal_escrever(Portal *p, int idx, float nova_nota, int novas_faltas){
    // Escritor precisa de exclusividade
    sem_wait(&p->recurso);

    // Seção crítica (escrita)
    p->alunos[idx].nota = nova_nota;
    p->alunos[idx].faltas = novas_faltas;
    logger_log(p->logger, "[Escritor] Atualizou id=%d nova_nota=%.1f faltas=%d",
               p->alunos[idx].id_aluno,
               nova_nota,
               novas_faltas);
    usleep(150000); // simula tempo de escrita

    sem_post(&p->recurso);
}
