# INF vs Zombies - Projeto Final INF01202

Projeto desenvolvido como trabalho final da disciplina **INF01202 - Algoritmos e Programação** (2025/1), ministrada pelo professor Marcelo Walter.

## 🌟 Visão Geral

INF vs Zombies é uma versão simplificada do clássico jogo Plants vs Zombies, implementada em **C** com o uso da biblioteca **Raylib**. O objetivo do jogo é impedir que os zumbis atravessem o gramado e cheguem à extremidade esquerda da tela.

## 🔧 Compilação e Execução

### Requisitos

* Compilador `gcc`
* Biblioteca `raylib` instalada
* `pkg-config` instalado

### Comandos

Compile, execute e limpe o binário com um único comando:

```bash
make run FILE=main.c OUTPUT=inf_zombies
```

Ou apenas compile:

```bash
make compile FILE=main.c OUTPUT=inf_zombies
```

## 📃 Regras do Jogo

* **Plantas:** Podem ser do tipo Dispara Ervilha ou Girassol

  * Disparam ervilhas a cada 4 segundos (dano: 20)
  * Girassol gera 1 sol a cada 4 segundos
* **Zumbis:**

  * Entram pelo lado direito da tela em hordas
  * Pontos de vida: 100
* **Cenário:**

  * Grid 5x9 (blocos de 72x96 pixels)
  * Cada zumbi morto vale 100 pontos

## 📁 Estrutura do Projeto

```
inf-vs-zombies/
├── assets/           # Sprites e recursos visuais
├── src/              # Arquivos fonte .c
├── include/          # Arquivos de cabeçalho .h
├── config.txt        # Configuração de hordas
├── top_scores.bin    # Ranking de jogadores
├── Makefile
└── README.md
```

## 📄 Funcionalidades Implementadas

* [ ] Menu inicial com três opções: Jogar, LeaderBoard e Sair
* [ ] Leitura do arquivo `config.txt` para definir as hordas
* [ ] Controle de plantas, zumbis e colisões
* [ ] Atualização e exibição do ranking (`top_scores.bin`)
* [ ] Interface gráfica com Raylib

## 🚀 Possíveis Expansões (Tarefas Extras)

* [ ] Novos tipos de plantas
* [ ] Novos tipos de zumbis (chefões, especiais, etc.)
* [ ] Menu de pause
* [ ] Novas formas de gerar sóis

## 🤝 Autores

* Nome do Aluno 1 - [André Schaidhauer Luckmann](mailto:601117@inf.ufrgs.br)
* Nome do Aluno 2 - [Arthur Von Groll dos Santos](mailto:602432@inf.ufrgs.br)
* Nome do Aluno 3 - [Vitor da Cunha Pimentel da Rosa](mailto:598732@inf.ufrgs.br)
