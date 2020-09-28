/*Programado por Oswaldo Schermach, Sistemas de Informação UTFPR */
/* segundo trabalho da disciplina de Fundamentos de Programação */

#include <stdio.h>
#include <stdlib.h>

#define LIN 31 //numero de linhas do campo
#define COL 31 //numero de colunas do campo

void simulacao(int mat[][COL],int matAux[][COL], int t);
void iniciaMatriz(int mat[][COL]);
void detectaVivos(int mat[][COL],int matAux[][COL],int linha,int coluna);
void imprimeMatriz(int mat[][COL]);
void testaMatriz(int mat[][COL],int matAux[][COL]);

void blinker(int mat[][COL]); //inicializa uma estrutura chamada blinker, são celulas que ficam oscilando ao longo de cada geração
void glider(int mat[][COL]); //inicializa uma estrutura chamada glider, eles se deslogam ao longo do tabuleiro a cada nova geração
void sapo(int mat[][COL]);  //inicializa uma estrutura chamada sapo, um outro tipo de oscilador
void randon(int mat[][COL]); //inicializa randomicamente uma populacao
void naveEspacial(int mat[][COL]); //inicializa uma estrutura chamada nave espacial, ele se desloca em linha reta ao longo do tabuleiro
void vidaEstavel(int mat[][COL]); // basicamente as formas mais chatas de vida, apenas existem, sem morrer nem evoluir



int main()
{
    srand(time(NULL));
    int mat[LIN][COL];
    int matAux[LIN][COL]; //matriz usada para salvar os dados da geração anterior
    int escolha;
    int t; //variavel usada para guardar o tempo de execucao da simulacao

    iniciaMatriz(mat); //inicializa a matriz com o numero 0
    iniciaMatriz(matAux); //inicializa a matriz com o numero 0

    //menu do jogo
    do{
        printf("\t\t\tJOGO DA VIDA\n\n\n");
        printf("\tEscolha sua opcao pressionando o numero correspondente no teclado, apos, tecle enter.\n\n");
        printf("\t\t(1) Blinker (Oscilador)\n\t\t(2) Glider\n\t\t(3) Sapo (oscilador)\n");
        printf("\t\t(4) Para uma inicializacao randomica\n\t\t(5) Nave Espacial\n\t\t(6) Vida Estavel\n\t\t(0) Para sair\n");
        printf("\nOpcao: ");
        scanf("%d",&escolha);
        system("cls");

        printf("\n\n\n\n\n\n\n\t\t\tEscolha o tempo de execucao:  ");
        scanf("%d",&t);

        switch(escolha)
        {
            case 1: blinker(mat);      simulacao(mat,matAux, t);      break;
            case 2: glider(mat);       simulacao(mat,matAux, t);      break;
            case 3: sapo(mat);         simulacao(mat,matAux, t);      break;
            case 4: randon(mat);       simulacao(mat,matAux, t);      break;
            case 5: naveEspacial(mat); simulacao(mat,matAux, t);      break;
            case 6: vidaEstavel(mat);  simulacao(mat,matAux, t);      break;
        }

    }while(escolha != 0);

    system("cls");
    printf("\n\n\n\n\n\n\n\t\t\tVoce escolheu sair.\n\n\n\n\n\n\n");
    system("Pause");


return 0;
}
//função usada para inicializar a matriz com numeros 0
void iniciaMatriz(int mat[][COL])
{
    int i,j;

    for(i = 0; i < LIN; i++)
    {
        for(j = 0; j < COL; j++)
            mat[i][j] = 0;
    }
}
/*esta função é usada para detectar quantas celulas vivas há na vizinhança de cada casa do tabuleiro,
assim ela atribui um valor para aquela casa de acordo com a quantidade de vivos próximos*/

/* Recebe como parametro a linha e a coluna, para que assim ela efetue a leitura de uma matriz 3X3 apenas
sem contabilizar o centro (que seria o elemento a ser avaliado)*/
void detectaVivos(int mat[][COL],int matAux[][COL],int linha,int coluna)
{
    int i, j;
    int contador = 0; /*variavel que atribui o valor para uma casa de acordo com a quantidade de vizinhos vivos (neste caso vivo quer dizer o numero "1"*/

    for(i = linha - 1; i <= linha + 1; i++)
    {
        for(j = coluna - 1; j <= coluna + 1; j++)
        {
            if(i != linha || j != coluna)
                if(mat[i][j] != 0)
                    contador++;
        }
    }
    matAux[linha][coluna] = contador; //atribui o numero de vizinhos para o elemento central, então copia este numero para a matriz auxiliar
}
/*Função para imprimr a matriz*/
void imprimeMatriz(int mat[][COL])
{
    int i,j;
    for(i = 1; i < LIN - 1; i++)
    {
        printf("\n");
        for(j = 1; j < COL - 1; j++)
            if(mat[i][j] != 0)
                printf("%3d", mat[i][j]);
            else printf("   ");
    }
}
/*Função que faz o teste de cada elemento da matriz, e decide quem vive e quem morre em cada elemento
sobrevivência: uma célula passa da geração corrente para a geração seguinte se ela tiver duas ou três células vizinhas vivas na geração corrente;
morte: uma célula viva morre ao fim da geração corrente se ela tem menos de duas (solidão) ou mais de três células vivas (inanição) na sua vizinhança;
nascimento: uma célula morta (re)nasce na geração seguinte caso ela tiver exatamente três células vivas na sua vizinhança na geração anterior.
*/
void testaMatriz(int mat[][COL],int matAux[][COL])
{
    int i, j;

    for(i = 0; i < LIN; i++)
    {
        for(j = 0; j < COL; j++)
        {
            if(matAux[i][j] < 2 || matAux[i][j] > 3)
                mat[i][j] = 0;
            if( matAux[i][j] == 3)
                mat[i][j] = 1;
        }
    }
}
/*Função que percorre a matriz elemento a elemento, então faz o envio da matriz 3X3 para a função que detecta vivos
após isso executa a função que faz os testes necessários */
void simulacao(int mat[][COL],int matAux[][COL], int t)
{
    int i, j;
    int tempo;

    for(tempo = 0;tempo < t; tempo++)
    {
        for(i = 1; i < LIN - 1; i++)
        {
            for(j = 1; j < COL - 1; j++)
                detectaVivos(mat,matAux, i, j);
        }
        testaMatriz(mat,matAux);
        imprimeMatriz(mat);
        system("cls");
    }
}
void blinker(int mat[][COL])
{

    mat[5][10] = 1;
    mat[5][11] = 1;
    mat[5][12] = 1;

    mat[15][15] = 1;
    mat[15][16] = 1;
    mat[15][17] = 1;
}
void glider(int mat[][COL])
{
    mat[15][15] = 1;
    mat[15][16] = 1;
    mat[15][17] = 1;
    mat[16][15] = 1;
    mat[17][17] = 1;
}
void sapo(int mat[][COL])
{
    mat[11][13] = 1;
    mat[11][14] = 1;
    mat[11][15] = 1;
    mat[12][12] = 1;
    mat[12][13] = 1;
    mat[12][14] = 1;
}
/*Percorre elemento a elemento preenchendo com 1 ou 0 aleatoriamente */
void randon(int mat[][COL])
{
    int i, j;

    for(i = 0; i < LIN; i++)
        for(j = 0; j < COL; j++)
            mat[i][j] = rand() % 2;

}
void naveEspacial(int mat[][COL])
{
    mat[15][16] = 1;
    mat[15][19] = 1;
    mat[16][15] = 1;
    mat[17][15] = 1;
    mat[17][19] = 1;
    mat[18][15] = 1;
    mat[18][16] = 1;
    mat[18][17] = 1;
    mat[18][18] = 1;
}
void vidaEstavel(int mat[][COL])
{
    mat[15][15] = 1;
    mat[15][16] = 1;
    mat[16][15] = 1;
    mat[16][16] = 1;

    mat[5][5] = 1;
    mat[5][6] = 1;
    mat[6][5] = 1;
    mat[6][6] = 1;

    mat[20][20] = 1;
    mat[20][21] = 1;
    mat[21][20] = 1;
    mat[21][22] = 1;
    mat[22][21] = 1;
}







