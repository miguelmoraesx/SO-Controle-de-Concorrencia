#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "portal.h"

#define TECLADO "TECLADO"
#define ARQUIVO "ARQUIVO"
#define ALEATORIO "ALEATORIO"
#define ENTRADA 1
#define TAM_MAXIMO_LINHA 256

int POS_THREADS = 2;
int QNT_THREADS = 0;

typedef struct {
    int qtd_alunos;
    RegistroAluno *alunos;
} DadosLidos;

void entradaTeclado();
DadosLidos* entradaArquivo(const char* local);
void entradaAleatoria();
void validaArquivo(const char* local);
void entradaDados(int argc, char** argv);

int main(int argc, char** argv){
    entradaDados(argc,argv);
    return 0;
}

void entradaDados(int argc,char** argv){
    if (argc < 3) {
        //caso de erro nos argumentos
        printf("Uso: %s [TECLADO|ARQUIVO|ALEATORIO] QNT_THREADS\n", argv[0]);
        exit(1);
    }

    if(!strcmp(argv[ENTRADA],TECLADO)){
        printf("ENTRADA VIA TECLADO ESCOLHIDO\n");
        entradaTeclado();
    }else if(!strcmp(argv[ENTRADA], ARQUIVO)){
        printf("ENTRADA VIA ARQUIVO ESCOLHIDO\n");
        validaArquivo(argv[2]);
        DadosLidos *d = entradaArquivo(argv[2]);
        //so pra ver se ta funcionando
        for(int i = 0; i < d->qtd_alunos; i++){
            printf("Aluno [%d]: ID=%d | Nota=%.1f | Faltas=%d\n", 
                   i,
                   d->alunos[i].id_aluno, 
                   d->alunos[i].nota, 
                   d->alunos[i].faltas);
        }
        POS_THREADS++;
    }else if(!strcmp(argv[ENTRADA], ALEATORIO)){
        printf("ENTRADA ALEATORIA ESCOLHIDA\n");
        entradaAleatoria();
    }else{
        //nenhuma entrada previsto foi escolhido
        printf("NENHUMA ENTRADA VALIDA ESCOLHIDA!\n");
        exit(1);
    }
}

void entradaTeclado(){
    //TAD AQUI
    //scanf()
}

DadosLidos* entradaArquivo(const char* local){
    FILE* f = fopen(local, "r");
    char linha[TAM_MAXIMO_LINHA];
    if(!f){
        printf("ERRO AO ABRIR O ARQUIVO %s\n", local);
        exit(1);
    }
    DadosLidos *dados = malloc(sizeof(DadosLidos));
    if(!dados){
        printf("ERRO AO ALOCAR MEMORIA");
        exit(1);
    }
    dados->qtd_alunos = 0;

    //le a qtd_alunos no arquivo
    while(fgets(linha, TAM_MAXIMO_LINHA, f) != NULL){
        char *carac = linha;
        //ignora espaco em branco
        while(*carac == ' ' || *carac == '\t') carac++;
        
        //ignora comentarios e linha vazia
        if(carac[0] == '#' || carac[0] == '\n' 
            || carac[0] == '\0') continue;

        //tenta ler a quantidade de alunos
        if(sscanf(carac, "%d", &dados->qtd_alunos) == 1){
            if(dados->qtd_alunos <= 0){
                printf("qtd_alunos menor que 0\n");
                fclose(f);
                free(dados);
                exit(1);
            }
            break;
        }else{
            printf("primeira linha nao tem qtd_alunos");
            fclose(f);
            free(dados);
            exit(1);
        }
    }

    //le os registros de alunos
    dados->alunos = malloc(sizeof(RegistroAluno)* dados->qtd_alunos);
    if(!dados->alunos){
        printf("erro pra alocar na leitura dos registros de alunos\n");
        fclose(f);
        free(dados);
        exit(1);
    }
    int alunos_lidos = 0;

    while(alunos_lidos < dados->qtd_alunos && fgets(linha, TAM_MAXIMO_LINHA, f) != NULL){
        char *carac = linha;
        //ignora espaco em branco
        while(*carac == ' ' || *carac == '\t') carac++;
        
        //ignora comentarios e linha vazia
        if(carac[0] == '#' || carac[0] == '\n' 
            || carac[0] == '\0') continue;


        // faz a leitura dos 3 campos de aluno
        if(sscanf(carac, "%d %f %d", &dados->alunos[alunos_lidos].id_aluno,
                                     &dados->alunos[alunos_lidos].nota,
                                     &dados->alunos[alunos_lidos].faltas) == 3)
            alunos_lidos++;
        //se nao leu 3 campos de aluno ele so ignora
    }
    if (alunos_lidos != dados->qtd_alunos) {
        printf("esperado %d alunos, lido apenas %d. Ajustando a contagem.\n", 
               dados->qtd_alunos, alunos_lidos);
        dados->qtd_alunos = alunos_lidos;
    }

    fclose(f);
    printf("Alunos lidos: %d\n", dados->qtd_alunos);
    return dados;
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