#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "input_output.h"

int main(int argc, char** argv){
    Portal* portal = malloc(sizeof(Portal));
    DadosLidos* dadosLidos = entradaDados(argc,argv, portal);

    /**
     * FALTA APLICAR THREADS
     * defini o id como constante 1 só pra testar
     */
    for(int i = 0; i < dadosLidos->qtd_alunos; i++){
        portal_escrever(portal, 1, i, dadosLidos->alunos[i].nota, dadosLidos->alunos[i].faltas);
    }
    for(int i = 0; i < dadosLidos->qtd_alunos; i++){
        portal_ler(portal, 1, i);
    }

    portal_close(portal);
    return 0;
}
