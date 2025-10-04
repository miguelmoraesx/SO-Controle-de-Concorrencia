.PHONY: compile p1_arquivo help

compile:
	gcc main.c portal.c logger.c -o main -pthread

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
