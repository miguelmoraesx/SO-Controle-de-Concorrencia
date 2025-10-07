.PHONY: compile p1_arquivo help

compile:
	gcc portal.c logger.c input_output.c thread_operacoes.c main.c -o main -pthread

FILE ?= dados.txt
THREADS ?= 4

all: compile p1_random

p1_arquivo: 
	./main ARQUIVO $(FILE)  $(THREADS)
p1_teclado:
	./main TECLADO $(THREADS)
p1_random:
	./main ALEATORIO $(THREADS)

help:
	@echo "Uso: make [função] [opções]"
	@echo "Funções:"
	@echo "  compile       - Compila o executável 'main' (Necessário antes de executar)."
	@echo "  p1_arquivo    - Executa o programa usando entrada de um arquivo."
	@echo "  p1_teclado    - Executa o programa usando entrada via teclado."
	@echo "  p1_random     - Executa o programa usando entrada aleatória."
	@echo ""
	@echo "Opções (podem ser definidas na linha de comando):"
	@echo "  FILE=...      - Especifica o nome do arquivo de entrada (por padrão usa: dados.txt)."
	@echo "  THREADS=...   - Especifica a quantidade total de threads (por padrão usa: 4)."
	@echo ""
	@echo "Exemplos:"
	@echo "  make compile"
	@echo "  make p1_arquivo FILE=alunos_grandes.txt THREADS=10"
	@echo "  make p1_teclado THREADS=5"
	@echo "  make p1_random"