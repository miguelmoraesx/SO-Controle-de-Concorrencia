#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "input_output.h"

int main(int argc, char** argv){
    Portal* portal = malloc(sizeof(Portal));
    entradaDados(argc,argv, portal);
    return 0;
}
