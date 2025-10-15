#include "portal.h"

Portal portal;

int portal_init(Portal *p, RegistroAluno *alunos, int qtd, Logger *lg){
    if (!p || !alunos || qtd <= 0) return 0;

    p->alunos = alunos;
    p->qtd_alunos = qtd;

    //nenhum controle de concorrencia
    p->leitores_ativos = 0;

    p->logger = lg;
    return 1;
}

void portal_close(Portal *p){
    //nada pra fazer
}

/* ---------------- Leitor (Aluno) ---------------- */
void portal_ler(Portal *p, int id_thread, int idx_aluno){
    if (!p || idx_aluno < 0 || idx_aluno >= p->qtd_alunos) return;

    // --- região crítica ---
    RegistroAluno r = p->alunos[idx_aluno];
    logger_log(p->logger, "[L-%02d] leu RA=%d  nota=%.1f  faltas=%d",
               id_thread, r.id_aluno, r.nota, r.faltas);
    // ------------------------------------------------

}

/* ---------------- Escritor (Professor) ---------------- */
void portal_escrever(Portal *p, int id_thread, int idx_aluno, float nova_nota, int delta_faltas){
    if (!p || idx_aluno < 0 || idx_aluno >= p->qtd_alunos) return;

    // --- região crítica ---
    if (nova_nota >= 0.0f)  p->alunos[idx_aluno].nota   = nova_nota;
    if (delta_faltas != 0)  p->alunos[idx_aluno].faltas += delta_faltas;

    logger_log(p->logger, "[E-%02d] escreveu RA=%d  nova_nota=%.1f  delta_faltas=%d",
               id_thread,
               p->alunos[idx_aluno].id_aluno,
               p->alunos[idx_aluno].nota,
               delta_faltas);
    // ----------------------------------
}
