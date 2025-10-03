/**
 * portal.h
 * ------------------------------------------
 * TAD Portal do Aluno (versão 1 - sem prioridade).
 *
 * Implementa o problema clássico dos leitores-escritores:
 * - Vários leitores podem acessar simultaneamente.
 * - Escritores têm exclusividade de acesso.
 *
 */
#ifndef PORTAL_H
#define PORTAL_H

#include <pthread.h>
#include <semaphore.h>
#include "logger.h"

/* Estrutura que representa um aluno */
typedef struct {
    int id_aluno;   // Identificador do aluno
    float nota;     // Nota do aluno
    int faltas;     // Número de faltas
} RegistroAluno;

/* Estrutura principal do Portal */
typedef struct {
    RegistroAluno *alunos;       // Vetor de registros
    int qtd_alunos;              // Quantidade de alunos

    int leitores;                // Contador de leitores ativos
    pthread_mutex_t mutex_leitores; // Protege contador de leitores
    sem_t recurso;               // Controla acesso exclusivo (diário)

    Logger *logger;              // Logger associado
} Portal;

/* Inicializa o portal com vetor de alunos */
int portal_init(Portal *p, RegistroAluno *alunos, int qtd, Logger *lg);

/* Encerra e libera recursos do portal */
void portal_close(Portal *p);

/* Operação de leitura (aluno consulta notas/faltas) */
void portal_ler(Portal *p, int idx);

/* Operação de escrita (professor altera notas/faltas) */
void portal_escrever(Portal *p, int idx, float nova_nota, int novas_faltas);

#endif
