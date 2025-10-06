#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "portal.h"
#include "logger.h"

#define TECLADO "TECLADO"
#define ARQUIVO "ARQUIVO"
#define ALEATORIO "ALEATORIO"
#define ENTRADA 1
#define TAM_MAXIMO_LINHA 256

typedef struct {
    int qtd_alunos;
    RegistroAluno *alunos;
} DadosLidos;

DadosLidos* entradaTeclado();
DadosLidos* entradaAleatoria();
void validaArquivo(const char* local);
DadosLidos* entradaDados(int argc, char** argv,  Portal *portal);
DadosLidos* entradaArquivo(const char* local);