.PHONY: all compile help

FILE ?= dados.txt
THREADS ?= 4

all: compile

compile:
	gcc portais/portal_V1.c buffers/buffer_V1.c util/logger.c util/input_output.c util/thread_operacoes.c main.c -o main_V1 -pthread
	gcc portais/portal_V2.c buffers/buffer_V2.c util/logger.c util/input_output.c util/thread_operacoes.c main.c -o main_V2 -pthread
	gcc portais/portal_V3.c buffers/buffer_V1.c util/logger.c util/input_output.c util/thread_operacoes.c main.c -o main_V3 -pthread

# Execuções por versão e tipo de entrada
v1_arquivo: main_V1
	./main_V1 ARQUIVO $(FILE) $(THREADS)

v1_teclado: main_V1
	./main_V1 TECLADO $(THREADS)

v1_random: main_V1
	./main_V1 ALEATORIO $(THREADS)

v2_arquivo: main_V2
	./main_V2 ARQUIVO $(FILE) $(THREADS)

v2_teclado: main_V2
	./main_V2 TECLADO $(THREADS)

v2_random: main_V2
	./main_V2 ALEATORIO $(THREADS)

v3_arquivo: main_V3
	./main_V3 ARQUIVO $(FILE) $(THREADS)

v3_teclado: main_V3
	./main_V3 TECLADO $(THREADS)

v3_random: main_V3
	./main_V3 ALEATORIO $(THREADS)

help:
	@echo "Uso: make [função] [opções]"
	@echo "Funções:"
	@echo "  compile       - Compila as 3 versões (main_V1, main_V2, main_V3)."
	@echo "  v1_arquivo    - Executa V1 usando entrada de arquivo."
	@echo "  v1_teclado    - Executa V1 usando teclado."
	@echo "  v1_random     - Executa V1 com entrada aleatória."
	@echo "  v2_arquivo    - Executa V2 usando entrada de arquivo."
	@echo "  v2_teclado    - Executa V2 usando teclado."
	@echo "  v2_random     - Executa V2 com entrada aleatória."
	@echo "  v3_arquivo    - Executa V3 usando entrada de arquivo."
	@echo "  v3_teclado    - Executa V3 usando teclado."
	@echo "  v3_random     - Executa V3 com entrada aleatória."
	@echo ""
	@echo "Opções (podem ser definidas na linha de comando):"
	@echo "  FILE=...      - Nome do arquivo de entrada (padrão: dados.txt)."
	@echo "  THREADS=...   - Quanti
