/*
 * Jogo da Vida (Conway) ? Oswaldo Schermach
 * Sistemas de Informacao UTFPR ? Fundamentos de Programacao
 *
 * Revitalizado: portabilidade Linux/Windows, double buffering e padroes corrigidos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define LIN 31
#define COL 31
#define ATRASO_MS 120

typedef int Tabuleiro[LIN][COL];

static void limpaTela(void);
static void pausa(void);
static void aguardaMs(int milissegundos);
static void limparBufferEntrada(void);

static void iniciaMatriz(Tabuleiro mat);
static int contaVizinhos(const Tabuleiro mat, int linha, int coluna);
static void proximaGeracao(const Tabuleiro atual, Tabuleiro proxima);
static void copiaTabuleiro(const Tabuleiro origem, Tabuleiro destino);
static int contaCelulasVivas(const Tabuleiro mat);

static void imprimeTabuleiro(const Tabuleiro mat, int geracao);
static void simulacao(Tabuleiro atual, Tabuleiro proxima, int geracoes);

static void blinker(Tabuleiro mat);
static void glider(Tabuleiro mat);
static void sapo(Tabuleiro mat);
static void aleatorio(Tabuleiro mat);
static void naveEspacial(Tabuleiro mat);
static void vidaEstavel(Tabuleiro mat);

static void defineCelula(Tabuleiro mat, int linha, int coluna);

int main(void)
{
    Tabuleiro atual;
    Tabuleiro proxima;
    int escolha;
    int geracoes;

    srand((unsigned)time(NULL));

    do {
        limpaTela();
        printf("\t\t\tJOGO DA VIDA\n\n");
        printf("\tConway's Game of Life ? tabuleiro %dx%d\n\n", LIN - 2, COL - 2);
        printf("\t(1) Blinker (oscilador)\n");
        printf("\t(2) Glider (navegador)\n");
        printf("\t(3) Sapo / Toad (oscilador)\n");
        printf("\t(4) Populacao aleatoria\n");
        printf("\t(5) Nave espacial (LWSS)\n");
        printf("\t(6) Vida estavel (blocos e colmeia)\n");
        printf("\t(0) Sair\n\n");
        printf("Opcao: ");

        if (scanf("%d", &escolha) != 1) {
            limparBufferEntrada();
            printf("\nEntrada invalida.\n");
            pausa();
            continue;
        }

        if (escolha == 0) {
            break;
        }

        limpaTela();
        printf("\nNumero de geracoes: ");
        if (scanf("%d", &geracoes) != 1 || geracoes <= 0) {
            limparBufferEntrada();
            printf("\nInforme um numero inteiro maior que zero.\n");
            pausa();
            continue;
        }

        iniciaMatriz(atual);
        iniciaMatriz(proxima);

        switch (escolha) {
        case 1:
            blinker(atual);
            break;
        case 2:
            glider(atual);
            break;
        case 3:
            sapo(atual);
            break;
        case 4:
            aleatorio(atual);
            break;
        case 5:
            naveEspacial(atual);
            break;
        case 6:
            vidaEstavel(atual);
            break;
        default:
            printf("\nOpcao invalida.\n");
            pausa();
            continue;
        }

        simulacao(atual, proxima, geracoes);
        pausa();
    } while (1);

    limpaTela();
    printf("\n\tObrigado por jogar. Ate a proxima!\n\n");
    pausa();

    return 0;
}

static void limpaTela(void)
{
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

static void pausa(void)
{
    printf("\nPressione Enter para continuar...");
    limparBufferEntrada();
    (void)getchar();
}

static void aguardaMs(int milissegundos)
{
    if (milissegundos <= 0) {
        return;
    }

#ifdef _WIN32
    Sleep((DWORD)milissegundos);
#else
    {
        struct timespec espera;

        espera.tv_sec = milissegundos / 1000;
        espera.tv_nsec = (long)(milissegundos % 1000) * 1000000L;
        nanosleep(&espera, NULL);
    }
#endif
}

static void limparBufferEntrada(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static void iniciaMatriz(Tabuleiro mat)
{
    int i;
    int j;

    for (i = 0; i < LIN; i++) {
        for (j = 0; j < COL; j++) {
            mat[i][j] = 0;
        }
    }
}

static void defineCelula(Tabuleiro mat, int linha, int coluna)
{
    if (linha > 0 && linha < LIN - 1 && coluna > 0 && coluna < COL - 1) {
        mat[linha][coluna] = 1;
    }
}

static int contaVizinhos(const Tabuleiro mat, int linha, int coluna)
{
    int i;
    int j;
    int contador = 0;

    for (i = linha - 1; i <= linha + 1; i++) {
        for (j = coluna - 1; j <= coluna + 1; j++) {
            if (i == linha && j == coluna) {
                continue;
            }
            if (mat[i][j] != 0) {
                contador++;
            }
        }
    }

    return contador;
}

/*
 * Regras de Conway:
 * - Sobrevivencia: 2 ou 3 vizinhos vivos
 * - Morte: menos de 2 (solidao) ou mais de 3 (superlotacao)
 * - Nascimento: celula morta com exatamente 3 vizinhos vivos
 */
static void proximaGeracao(const Tabuleiro atual, Tabuleiro proxima)
{
    int i;
    int j;
    int vizinhos;

    for (i = 1; i < LIN - 1; i++) {
        for (j = 1; j < COL - 1; j++) {
            vizinhos = contaVizinhos(atual, i, j);

            if (atual[i][j] != 0) {
                proxima[i][j] = (vizinhos == 2 || vizinhos == 3);
            } else {
                proxima[i][j] = (vizinhos == 3);
            }
        }
    }

    for (i = 0; i < LIN; i++) {
        proxima[0][i] = 0;
        proxima[LIN - 1][i] = 0;
        proxima[i][0] = 0;
        proxima[i][COL - 1] = 0;
    }
}

static void copiaTabuleiro(const Tabuleiro origem, Tabuleiro destino)
{
    int i;
    int j;

    for (i = 0; i < LIN; i++) {
        for (j = 0; j < COL; j++) {
            destino[i][j] = origem[i][j];
        }
    }
}

static int contaCelulasVivas(const Tabuleiro mat)
{
    int i;
    int j;
    int total = 0;

    for (i = 1; i < LIN - 1; i++) {
        for (j = 1; j < COL - 1; j++) {
            if (mat[i][j] != 0) {
                total++;
            }
        }
    }

    return total;
}

static void imprimeTabuleiro(const Tabuleiro mat, int geracao)
{
    int i;
    int j;

    limpaTela();
    printf("\t\tJOGO DA VIDA ? geracao %d\n", geracao);
    printf("\t\tCelulas vivas: %d\n\n", contaCelulasVivas(mat));

    for (i = 1; i < LIN - 1; i++) {
        printf("\t");
        for (j = 1; j < COL - 1; j++) {
            printf(mat[i][j] ? "[]" : "  ");
        }
        printf("\n");
    }

    fflush(stdout);
}

static void simulacao(Tabuleiro atual, Tabuleiro proxima, int geracoes)
{
    int g;

    imprimeTabuleiro(atual, 0);
    aguardaMs(ATRASO_MS * 2);

    for (g = 1; g <= geracoes; g++) {
        proximaGeracao(atual, proxima);
        copiaTabuleiro(proxima, atual);
        imprimeTabuleiro(atual, g);
        aguardaMs(ATRASO_MS);
    }
}

static void blinker(Tabuleiro mat)
{
    defineCelula(mat, 5, 10);
    defineCelula(mat, 5, 11);
    defineCelula(mat, 5, 12);

    defineCelula(mat, 15, 15);
    defineCelula(mat, 15, 16);
    defineCelula(mat, 15, 17);
}

static void glider(Tabuleiro mat)
{
    /* Glider canonico:
     *  . X .
     *  . . X
     *  X X X
     */
    defineCelula(mat, 15, 16);
    defineCelula(mat, 16, 17);
    defineCelula(mat, 17, 15);
    defineCelula(mat, 17, 16);
    defineCelula(mat, 17, 17);
}

static void sapo(Tabuleiro mat)
{
    /* Toad (periodo 2):
     *  . X X X
     *  X X X .
     */
    defineCelula(mat, 11, 13);
    defineCelula(mat, 11, 14);
    defineCelula(mat, 11, 15);
    defineCelula(mat, 12, 12);
    defineCelula(mat, 12, 13);
    defineCelula(mat, 12, 14);
}

static void aleatorio(Tabuleiro mat)
{
    int i;
    int j;

    for (i = 1; i < LIN - 1; i++) {
        for (j = 1; j < COL - 1; j++) {
            mat[i][j] = rand() % 2;
        }
    }
}

static void naveEspacial(Tabuleiro mat)
{
    /* Lightweight spaceship (LWSS) */
    defineCelula(mat, 15, 16);
    defineCelula(mat, 15, 18);
    defineCelula(mat, 16, 14);
    defineCelula(mat, 17, 14);
    defineCelula(mat, 17, 18);
    defineCelula(mat, 18, 15);
    defineCelula(mat, 18, 16);
    defineCelula(mat, 18, 17);
    defineCelula(mat, 18, 18);
    defineCelula(mat, 19, 14);
    defineCelula(mat, 19, 18);
}

static void vidaEstavel(Tabuleiro mat)
{
    /* Blocos 2x2 */
    defineCelula(mat, 5, 5);
    defineCelula(mat, 5, 6);
    defineCelula(mat, 6, 5);
    defineCelula(mat, 6, 6);

    defineCelula(mat, 15, 15);
    defineCelula(mat, 15, 16);
    defineCelula(mat, 16, 15);
    defineCelula(mat, 16, 16);

    /* Colmeia (beehive) */
    defineCelula(mat, 20, 20);
    defineCelula(mat, 20, 21);
    defineCelula(mat, 21, 19);
    defineCelula(mat, 21, 22);
    defineCelula(mat, 22, 20);
    defineCelula(mat, 22, 21);
}
