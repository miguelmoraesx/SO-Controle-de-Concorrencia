.PHONY: all compile help

FILE ?= dados.txt
THREADS ?= 4

all: compile

compile:
	gcc portais/portal_V1.c buffers/buffer_V1.c buffers/buffer_V2.c buffers/buffer_V3.c util/logger.c util/input_output.c util/thread_operacoes.c main.c -o main_V1 -pthread
	gcc portais/portal_V2.c buffers/buffer_V1.c buffers/buffer_V2.c buffers/buffer_V3.c util/logger.c util/input_output.c util/thread_operacoes.c main.c -o main_V2 -pthread
	gcc portais/portal_V3.c buffers/buffer_V1.c buffers/buffer_V2.c buffers/buffer_V3.c util/logger.c util/input_output.c util/thread_operacoes.c main.c -o main_V3 -pthread

# -----------------------------
# Versão 1 - portal
v1_arquivo: main_V1
	./main_V1 portal ARQUIVO $(FILE) $(THREADS)

v1_teclado: main_V1
	./main_V1 portal TECLADO $(THREADS)

v1_random: main_V1
	./main_V1 portal ALEATORIO $(THREADS)

# Versão 1 - buffer
v1_buffer: main_V1
	./main_V1 buffer_v1 $(THREADS)

# -----------------------------
# Versão 2 - portal
v2_arquivo: main_V2
	./main_V2 portal ARQUIVO $(FILE) $(THREADS)

v2_teclado: main_V2
	./main_V2 portal TECLADO $(THREADS)

v2_random: main_V2
	./main_V2 portal ALEATORIO $(THREADS)

# Versão 2 - buffer
v2_buffer: main_V2
	./main_V2 buffer_v2 $(THREADS)

# -----------------------------
# Versão 3 - portal
v3_arquivo: main_V3
	./main_V3 portal ARQUIVO $(FILE) $(THREADS)

v3_teclado: main_V3
	./main_V3 portal TECLADO $(THREADS)

v3_random: main_V3
	./main_V3 portal ALEATORIO $(THREADS)

# Versão 3 - buffer
v3_buffer: main_V3
	./main_V3 buffer_v3 $(THREADS)

# -----------------------------
help:
	@echo "Uso: make [função] [opções]"
	@echo "Funções:"
	@echo "  compile       - Compila as 3 versões (main_V1, main_V2, main_V3)."
	@echo "  v1_arquivo    - Executa V1 via portal usando arquivo."
	@echo "  v1_teclado    - Executa V1 via portal usando teclado."
	@echo "  v1_random     - Executa V1 via portal com entrada aleatória."
	@echo "  v1_buffer     - Executa V1 direto pelo buffer (só threads)."
	@echo "  v2_arquivo    - Executa V2 via portal usando arquivo."
	@echo "  v2_teclado    - Executa V2 via portal usando teclado."
	@echo "  v2_random     - Executa V2 via portal com entrada aleatória."
	@echo "  v2_buffer     - Executa V2 direto pelo buffer (só threads)."
	@echo "  v3_arquivo    - Executa V3 via portal usando arquivo."
	@echo "  v3_teclado    - Executa V3 via portal usando teclado."
	@echo "  v3_random     - Executa V3 via portal com entrada aleatória."
	@echo "  v3_buffer     - Executa V3 direto pelo buffer (só threads)."
	@echo ""
	@echo "Opções (podem ser definidas na linha de comando):"
	@echo "  FILE=...      - Nome do arquivo de entrada (padrão: dados.txt)."
	@echo "  THREADS=...   - Quantidade de threads (padrão: 4)."
