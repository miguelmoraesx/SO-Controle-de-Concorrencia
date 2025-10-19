# 🧩 Portal do Aluno — Controle de Concorrência em Sistemas Operacionais

Projeto desenvolvido para a disciplina de **Sistemas Operacionais (UFAM)**, com o objetivo de implementar o **problema clássico dos Leitores e Escritores**, aplicado em um cenário realista chamado **Portal do Aluno**.

---

## 👥 Equipe

- **Miguel Oliveira Moraes de Souza**  
- **Pedro Henrique Belota Gadelha**  
- **André Kaled Coutinho**

---

## 🎯 Objetivo do Projeto

O trabalho simula o funcionamento de um **Portal do Aluno**, onde várias threads (usuários) podem:
- **Ler informações** dos alunos (como notas e faltas);  
- **Escrever informações** (atualizar notas ou faltas);  

O desafio é garantir que:
- Vários **leitores** possam acessar o portal **simultaneamente**,  
- Mas apenas **um escritor** (professor) possa alterar os dados **por vez**,  
evitando conflitos e mantendo a integridade das informações.

---

## ⚙️ Estrutura do Projeto

