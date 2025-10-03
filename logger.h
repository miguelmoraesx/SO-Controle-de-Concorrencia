/**
 * logger.h
 * ------------------------------------------
 * Módulo de LOG thread-safe para registrar eventos
 * de leitores e escritores no Portal do Aluno.
 *
 * Cada função utiliza mutex para garantir que
 * múltiplas threads não misturem mensagens na saída.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>
#include <stdbool.h>
#include <stdarg.h>

/* Estrutura do logger */
typedef struct {
    pthread_mutex_t lock;   // Protege a escrita concorrente
    bool habilitado;        // Liga/desliga os logs
} Logger;

/* Inicializa o logger */
void logger_init(Logger *lg);

/* Fecha o logger e libera recursos */
void logger_close(Logger *lg);

/* Registra uma mensagem formatada (como printf) */
void logger_log(Logger *lg, const char *fmt, ...);

#endif
