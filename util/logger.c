/**
 * logger.c
 * Implementação simples do sistema de LOG thread-safe.
 *
 * Esse módulo serve pra registrar o que cada thread tá fazendo,
 * evitando que as mensagens se misturem no terminal.
 * Usa um mutex pra garantir que só uma thread escreva por vez.
 */

#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

/**
 * Inicializa o logger.
 * Cria o mutex e habilita o log.
 */
void logger_init(Logger *lg) {
    pthread_mutex_init(&lg->lock, NULL);
    lg->habilitado = true;
}

/**
 * Fecha o logger.
 * Destroi o mutex e libera recursos.
 */
void logger_close(Logger *lg) {
    pthread_mutex_destroy(&lg->lock);
}

/**
 * Escreve uma mensagem no log de forma segura (thread-safe).
 * Aceita formato tipo printf.
 */
void logger_log(Logger *lg, const char *fmt, ...) {
    if (!lg->habilitado) return;

    pthread_mutex_lock(&lg->lock);

    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    printf("\n");
    fflush(stdout); // garante que a linha aparece na hora
    va_end(ap);

    pthread_mutex_unlock(&lg->lock);
}
