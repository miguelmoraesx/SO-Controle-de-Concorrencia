#include "input_output.h"

int POS_THREADS = 2;
int QNT_THREADS = 0;

Logger logger;

void alocarAlunos(DadosLidos *dados, int qnt, Logger logger){
    if(qnt <= 0){
        dados->alunos = NULL;
        dados->qtd_alunos = 0;
        return;
    }

    dados->qtd_alunos = qnt;
    dados->alunos = (RegistroAluno*) malloc(dados->qtd_alunos * sizeof(RegistroAluno));

    if(dados->alunos == NULL){
        logger_log(&logger, "[ERRO] alocar memoria para alunos retornou NULL");
        dados->qtd_alunos = 0;
        exit(1);
    }
}

DadosLidos* entradaDados(int argc,char** argv, Portal *portal){
    logger_init(&logger);
    logger_log(&logger, "Iniciando execucao...");
    DadosLidos *dados;
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

        //instancia dados para a entrada via arquivo
        dados = entradaArquivo(argv[2]);
        logger_log(&logger, "\n--- Alunos carregados do arquivo (%d) ---", dados->qtd_alunos);
        for(int i=0;i<dados->qtd_alunos;i++){
            logger_log(&logger, 
                       "Aluno [%d]: ID=%d | Nota=%.2f | Faltas=%d", 
                       i,
                       dados->alunos[i].id_aluno, 
                       dados->alunos[i].nota, 
                       dados->alunos[i].faltas);
        }
        logger_log(&logger, "------------------------------------------");
        if(portal_init(portal, dados->alunos, dados->qtd_alunos, &logger)){
            logger_log(&logger, "[Portal] Portal iniciado com sucesso!");
        }else{
            logger_log(&logger, "[Erro] Falha ao iniciar o portal!");
            free(dados->alunos);
            free(dados);
            logger_close(&logger);
            exit(1);
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
    QNT_THREADS = atoi(argv[POS_THREADS]);
    if (QNT_THREADS <= 0) {
        logger_log(&logger, "[ERRO] QNT_THREADS deve ser maior que zero.");
        exit(1);
    }
    return dados;
}

DadosLidos* entradaTeclado(){
    //TAD AQUI
    //scanf()
    return NULL;
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
    alocarAlunos(dados, dados->qtd_alunos, logger);
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

DadosLidos* entradaAleatoria(){
    srand(time(NULL));
    //gera aleatorioamente pra passar pro TAD
    return NULL;
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