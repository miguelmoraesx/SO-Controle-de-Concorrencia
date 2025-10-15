/**
 * logger.c
 * ------------------------------------------
 * Implementação do sistema de LOG.
 */
#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

void logger_init(Logger *lg){
    pthread_mutex_init(&lg->lock, NULL);
    lg->habilitado = true;
}

void logger_close(Logger *lg){
    pthread_mutex_destroy(&lg->lock);
}

void logger_log(Logger *lg, const char *fmt, ...){
    if(!lg->habilitado) return;

    pthread_mutex_lock(&lg->lock);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
    pthread_mutex_unlock(&lg->lock);
}
