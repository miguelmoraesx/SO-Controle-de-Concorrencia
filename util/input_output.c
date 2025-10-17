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

DadosLidos* entradaDados(int argc, char** argv, Portal *portal) {
    logger_init(&logger);
    logger_log(&logger, "Iniciando execucao...");

    if (argc < 4) {
        printf("Uso esperado: %s portal [TECLADO|ALEATORIO] QNT_THREADS\n", argv[0]);
        printf("Uso esperado: %s portal ARQUIVO [ARQUIVO_DADOS] QNT_THREADS\n", argv[0]);
        logger_log(&logger, "[ERRO] Argumentos insuficientes.");
        exit(1);
    }

    char* tipo_entrada = argv[2];

    if (!strcmp(tipo_entrada, TECLADO) || !strcmp(tipo_entrada, ALEATORIO)) {
        POS_THREADS = 3; 
    } else if (!strcmp(tipo_entrada, ARQUIVO)) {
        POS_THREADS = 4;
    } else {
        printf("NENHUMA ENTRADA VALIDA ESCOLHIDA!\n");
        exit(1);
    }

    // Verifica se QNT_THREADS foi passado
    if (argc <= POS_THREADS) {
        logger_log(&logger, "[ERRO] Argumento QNT_THREADS ausente ou invalido.");
        exit(1);
    }

    QNT_THREADS = atoi(argv[POS_THREADS]);
    if (QNT_THREADS <= 0) {
        logger_log(&logger, "[ERRO] QNT_THREADS deve ser um numero inteiro maior que zero.");
        exit(1);
    }

    DadosLidos *dados = NULL;

    if (!strcmp(tipo_entrada, TECLADO)) {
        dados = entradaTeclado();
    } else if (!strcmp(tipo_entrada, ARQUIVO)) {
        if (argc < 5) {
            logger_log(&logger, "[ERRO] Nome do arquivo ausente.");
            exit(1);
        }
        validaArquivo(argv[3]);
        dados = entradaArquivo(argv[3]);
    } else if (!strcmp(tipo_entrada, ALEATORIO)) {
        dados = entradaAleatoria();
    }

    if (dados == NULL) {
        logger_log(&logger, "[ERRO] Falha critica na alocacao/leitura de dados.");
        logger_close(&logger);
        exit(1);
    }

    logger_log(&logger, "\n--- Alunos carregados (%d) ---", dados->qtd_alunos);
    if (dados->alunos) {
        for(int i=0;i<dados->qtd_alunos;i++){
            logger_log(&logger, 
                       "Aluno [%d]: ID=%d | Nota=%.2f | Faltas=%d", 
                       i,
                       dados->alunos[i].id_aluno, 
                       dados->alunos[i].nota, 
                       dados->alunos[i].faltas);
        }
    }
    logger_log(&logger, "------------------------------------------");

    if(portal_init(portal, dados->alunos, dados->qtd_alunos, &logger)){
        logger_log(&logger, "[Portal] Portal iniciado com sucesso!");
    } else {
        logger_log(&logger, "[Erro] Falha ao iniciar o portal!");
        if (dados->alunos) free(dados->alunos);
        free(dados);
        logger_close(&logger);
        exit(1);
    }

    return dados;
}

DadosLidos* entradaTeclado(){
    DadosLidos *dados = malloc(sizeof(DadosLidos));
    if (!dados) {
        printf("ERRO AO ALOCAR MEMORIA PARA DADOS TECLADO\n");
        exit(1);
    }

    printf("Quantos alunos deseja cadastrar? ");
    // Lê a quantidade e verifica se é válida
    if (scanf("%d", &dados->qtd_alunos) != 1 || dados->qtd_alunos <= 0) {
        printf("Quantidade de alunos inválida. Deve ser maior que zero.\n");
        free(dados);
        exit(1);
    }
    
    // Aloca a memória para o array de alunos
    alocarAlunos(dados, dados->qtd_alunos, logger); 
    
    for (int i = 0; i < dados->qtd_alunos; i++) {
        printf("Aluno %d (ID, Nota, Faltas): ", i + 1);
        if (scanf("%d %f %d", &dados->alunos[i].id_aluno,
                             &dados->alunos[i].nota,
                             &dados->alunos[i].faltas) != 3) {
            printf("Erro ao ler dados do aluno %d\n", i + 1);
            free(dados->alunos);
            free(dados);
            exit(1);
        }
    }

    printf("Alunos lidos: %d\n", dados->qtd_alunos);
    return dados;
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
    DadosLidos *dados = malloc(sizeof(DadosLidos));
    if (!dados) {
        printf("ERRO AO ALOCAR MEMORIA PARA DADOS ALEATORIOS\n");
        exit(1);
    }

    srand(time(NULL));
    int qtd = 10; // Valor fixo para teste
    dados->qtd_alunos = qtd;

    alocarAlunos(dados, qtd, logger);

    for (int i = 0; i < qtd; i++) {
        dados->alunos[i].id_aluno = i + 1;
        // Gera nota de 0.0 a 10.0 com 1 casa decimal
        dados->alunos[i].nota = ((float)(rand() % 101)) / 10.0; 
        dados->alunos[i].faltas = rand() % 6; // 0 a 5 faltas
    }

    printf("Alunos gerados aleatoriamente: %d\n", qtd);
    return dados;
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