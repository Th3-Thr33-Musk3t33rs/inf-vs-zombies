# INF vs Zombies - Projeto Final INF01202

Projeto desenvolvido como trabalho final da disciplina **INF01202 - Algoritmos e Programação** (2025/1), ministrada pelo professor Marcelo Walter.

## Visão Geral

INF vs Zombies é uma versão simplificada do clássico jogo Plants vs Zombies, implementada em **C** com o uso da biblioteca **Raylib**. O objetivo do jogo é impedir que os zumbis atravessem o gramado e cheguem à extremidade esquerda da tela.

## Compilação e Execução

### Requisitos

* Compilador `gcc`
* Biblioteca `raylib` instalada
* `pkg-config` instalado

### Comandos

Compile, execute e limpe o binário com um único comando:

```bash
make run
```

## Regras do Jogo

* **Plantas:**
  * Peashooter: Dispara contra zumbis em cadência constante.
  * Girassol: Gera dinheiro em um intervalo de tempo constante.
  * Batata: Tem mais vida e serve para atrasar os zumbis, como um muro.
  * Mina: Mata o zumbi que pisar na tile em que ela for plantada.
  * Carnívora: Devora o zumbi que estiver no quadrado a frente e entra em cooldown.
* **Zumbis:**
  * Entram pelo lado direito da tela em hordas
  * Pontos de vida: 100
* **Cenário:**
  * Grid 7x9 (blocos de 96x78 pixels)
  * Cada zumbi morto vale 100 pontos

## Estrutura do Projeto

```
inf-vs-zombies/
├── .vscode/          # Configurações da IDE
├── assets/           # Sprites e recursos visuais
├── include/          # Arquivos de cabeçalho .h
├── src/              # Arquivos fonte .c
├── .clan-format      # Formatter com convenções de estilo
├── config.txt        # Configuração de hordas
├── top_scores.bin    # Ranking de jogadores
├── Makefile          # Automação de scripts (make run, make clean)
└── README.md
```

## Funcionalidades Implementadas

* [X] Menu inicial com três opções: Jogar, LeaderBoard e Sair
* [X] Leitura do arquivo `config.txt` para definir as hordas
* [X] Controle de plantas, zumbis e colisões
* [X] Atualização e exibição do ranking (`top_scores.bin`)
* [X] Interface gráfica com Raylib

## Possíveis Expansões (Tarefas Extras)

* [X] Novos tipos de plantas
* [X] Novos tipos de zumbis
* [X] Menu de pause
* [X] Novas formas de gerar sóis

## Autores

* Nome do Aluno 1 - [André Schaidhauer Luckmann](mailto:601117@inf.ufrgs.br)
* Nome do Aluno 2 - [Arthur Von Groll dos Santos](mailto:602432@inf.ufrgs.br)
* Nome do Aluno 3 - [Vitor da Cunha Pimentel da Rosa](mailto:598732@inf.ufrgs.br)
