# 🧩 Controle de Concorrência em Sistemas Operacionais

Projeto desenvolvido para a disciplina de **Sistemas Operacionais (UFAM)**, abordando os **problemas clássicos de concorrência** — Leitores e Escritores, e Produtor e Consumidor — utilizando **Threads**, **Semáforos** e **Mutex** em linguagem **C**.

---

## 👥 Equipe

- **Miguel Oliveira Moraes de Souza**  
- **Pedro Henrique Belota Gadelha**  
- **André Kaled Coutinho**

---

## 🧵 Descrição

O trabalho tem como objetivo **simular a sincronização de processos concorrentes**, garantindo a integridade do acesso a recursos compartilhados.

O projeto é dividido em **duas partes principais**:

### 🧩 Parte 1 — Leitores e Escritores
Implementa o controle de acesso onde múltiplos leitores podem ler simultaneamente, mas apenas **um escritor** pode modificar o recurso por vez.  
O sistema utiliza **mutex** e **semaforos** para coordenar o acesso e evitar condições de corrida.

### ⚙️ Parte 2 — Buffer Circular (Produtor e Consumidor)
Simula a comunicação entre **produtores** (que inserem dados no buffer) e **consumidores** (que retiram dados).  
O controle de concorrência garante que o buffer nunca fique **cheio** ou **vazio**, respeitando as restrições de sincronização.

---

## 🧰 Estrutura de Pastas

