#include "portal.h"

Portal portal;

int portal_init(Portal *p, RegistroAluno *alunos, int qtd, Logger *lg){
    if (!p || !alunos || qtd <= 0) return 0;

    p->alunos = alunos;
    p->qtd_alunos = qtd;

    p->leitores_ativos = 0;
    pthread_mutex_init(&p->mtx_leitores, NULL);

    //somente o semaforo do recurso
    sem_init(&p->recurso,   0, 1);

    p->logger = lg;
    return 1;
}

void portal_close(Portal *p){
    if (!p) return;
    pthread_mutex_destroy(&p->mtx_leitores);
    sem_destroy(&p->recurso);
}

/* ---------------- Leitor (Aluno) ---------------- */
void portal_ler(Portal *p, int id_thread, int idx_aluno){
    if (!p || idx_aluno < 0 || idx_aluno >= p->qtd_alunos) return;

    pthread_mutex_lock(&p->mtx_leitores);
    p->leitores_ativos++;
    if (p->leitores_ativos == 1) //primeiro leitor bloqueia recurso
        sem_wait(&p->recurso);
    pthread_mutex_unlock(&p->mtx_leitores);

    // --- região crítica ---
    RegistroAluno r = p->alunos[idx_aluno];
    logger_log(p->logger, "[L-%02d] leu RA=%d  nota=%.1f  faltas=%d",
               id_thread, r.id_aluno, r.nota, r.faltas);
    // ------------------------------------------------

    pthread_mutex_lock(&p->mtx_leitores);
    p->leitores_ativos--;
    if (p->leitores_ativos == 0) //ultimo leitor libera o recurso
        sem_post(&p->recurso);
    pthread_mutex_unlock(&p->mtx_leitores);
}

/* ---------------- Escritor (Professor) ---------------- */
void portal_escrever(Portal *p, int id_thread, int idx_aluno, float nova_nota, int delta_faltas){
    if (!p || idx_aluno < 0 || idx_aluno >= p->qtd_alunos) return;

    //bloqueia o recurso (1 escritor por vez)
    sem_wait(&p->recurso);

    // --- região crítica ---
    if (nova_nota >= 0.0f)  p->alunos[idx_aluno].nota   = nova_nota;
    if (delta_faltas != 0)  p->alunos[idx_aluno].faltas += delta_faltas;

    logger_log(p->logger, "[E-%02d] escreveu RA=%d  nova_nota=%.1f  delta_faltas=%d",
               id_thread,
               p->alunos[idx_aluno].id_aluno,
               p->alunos[idx_aluno].nota,
               delta_faltas);
    // ----------------------------------

    sem_post(&p->recurso);
}
