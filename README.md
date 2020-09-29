# Jogo da Vida

Implementacao do **Jogo da Vida de Conway** em C, originalmente desenvolvida como trabalho da disciplina de Fundamentos de Programacao (UTFPR).

## Requisitos

- Compilador C compativel com C99 (`gcc` ou `clang`)

## Compilacao

```bash
make
```

Ou manualmente:

```bash
gcc -Wall -Wextra -std=c99 -O2 -D_POSIX_C_SOURCE=200809L -o jogo_da_vida Jogo_da_Vida.c
```

## Execucao

```bash
make run
```

Ou:

```bash
./jogo_da_vida
```

## Padroes disponiveis

| Opcao | Padrao | Comportamento |
|-------|--------|---------------|
| 1 | Blinker | Oscilador periodo 2 |
| 2 | Glider | Navega pelo tabuleiro |
| 3 | Sapo (Toad) | Oscilador periodo 2 |
| 4 | Aleatorio | Populacao inicial aleatoria |
| 5 | Nave espacial | Lightweight spaceship (LWSS) |
| 6 | Vida estavel | Blocos e colmeia |

## Regras

- **Sobrevivencia:** celula viva com 2 ou 3 vizinhos vivos
- **Morte:** celula viva com menos de 2 ou mais de 3 vizinhos
- **Nascimento:** celula morta com exatamente 3 vizinhos vivos

O tabuleiro usa bordas fixas (sem torus): a area jogavel e 29x29 celulas.

## Autor

Oswaldo Schermach — Sistemas de Informacao UTFPR
