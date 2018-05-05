#ifndef ESTRUTURAS_H_INCLUDED
#define ESTRUTURAS_H_INCLUDED

#include "lista_encadeada.h"

typedef struct {
    /* Dimens�es: */
    int l; // largura
    int a; // altura
    int p; // profundidade
} caixa;

typedef struct {
    /* Tamanho do problema: */
    int n;
    /* Altura m�xima da pilha: */
    int h; // altura
    /* Conjunto de caixas: */
    caixa* caixas;
    /* Valores associados �s caixas: */
    int* v;

    // A vari�vel i_caixas � usada para controle interno,
    // informando quantas caixas j� foram adicionadas ao
    // problema.
    int i_caixas;

    // A vari�vel i_v � usada para controle interno,
    // informando quantos valores j� foram adicionados ao
    // problema.
    int i_v;
} problema_empilhamento;

typedef problema_empilhamento* problema;

problema cria_problema(int, int);

void termina_problema(problema);

int add_caixa(problema, caixa);

int add_valor(problema, int);

int empilhamento_bottom_up(problema, int**, int*);

int empilhamento_top_down(problema, int**, int*);

#endif // ESTRUTURAS_H_INCLUDED
