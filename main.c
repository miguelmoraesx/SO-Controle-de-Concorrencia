#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TECLADO "TECLADO"
#define ARQUIVO "ARQUIVO"
#define ALEATORIO "ALEATORIO"
#define ENTRADA 1

void entradaTeclado();
void entradaArquivo(const char* local);
void entradaAleatoria();
void validaArquivo(const char* local);

int main(int argc, char** argv){
    int POS_THREADS = 2;
    int QNT_THREADS = 0;

    if (argc < 3) {
        printf("Uso: %s [TECLADO|ARQUIVO|ALEATORIO] QNT_THREADS\n", argv[0]);
        return 1;
    }

    if(!strcmp(argv[ENTRADA],TECLADO)){
        printf("ENTRADA VIA TECLADO ESCOLHIDO\n");
        entradaTeclado();
    }else if(!strcmp(argv[ENTRADA], ARQUIVO)){
        printf("ENTRADA VIA ARQUIVO ESCOLHIDO\n");
        validaArquivo(argv[2]);
        entradaArquivo(argv[2]);
        POS_THREADS++;
    }else if(!strcmp(argv[ENTRADA], ALEATORIO)){
        printf("ENTRADA ALEATORIA ESCOLHIDA\n");
        entradaAleatoria();
    }else{
        printf("NENHUMA ENTRADA VALIDA ESCOLHIDA!\n");
        return 1;
    }
    return 0;
}

void entradaTeclado(){
    //TAD AQUI
    //scanf()
}

void entradaArquivo(const char* local){
    FILE* f = fopen(local, "r");
    if(!f){
        printf("ERRO AO ABRIR O ARQUIVO %s\n", local);
        exit(1);
    }
    //le arquivo e passa pro TAD
}

void entradaAleatoria(){
    srand(time(NULL));
    //gera aleatorioamente pra passar pro TAD
}

void validaArquivo(const char* local){
    FILE* f = fopen(local, "r");
    if(!f){
        printf("ERRO AO ABRIR O ARQUIVO %s\n", local);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fclose(f);

    if(!tam){
        printf("O ARQUIVO %s ESTÁ VAZIO!\n", local);
        exit(1);
    }
    printf("ARQUIVO VALIDADO COM SUCESSO!\n");
}