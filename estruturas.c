#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"

/** ASSINATURAS DE FUN��ES INTERNAS, CUJAS IMPLEMENTA��ES SE ENCONTRAM NO
  * FINAL DO ARQUIVO.
  */
int* cria_vetor_caixas_empilhadas(int**, caixa*, int, int, int*);

lista* cria_vetor_lista_de_caixas_empilhaveis(caixa*, int);

void termina_vetor_lista_de_caixas_empilhaveis(lista*, int);

int max(int*, int, int*);

/* Esta fun��o aloca espa�o em mem�ria para uma inst�ncia do TAD problema.
 * Os par�metros de entrada s�o n (o tamanho do problema, ou seja, o n�mero
 * de caixas) e h (altura m�xima da pilha de caixas).
 *
 * Retorna uma inst�ncia do TAD problema ou retorna NULL, caso haja problemas
 * na aloca��o din�mica de mem�ria ou se n menor ou igual a zero.
 *
 * Complexidade: O(1)
 */
problema cria_problema(int n, int h)
{
    problema p = NULL; // Vari�vel que ser� retornada ao final da fun��o.

    // Verifica se o valor de n � v�lido.
    if (n > 0) {

        // Aloca espa�o em mem�ria.
        p = (problema) malloc(sizeof(problema_empilhamento));

        // Verifica se a aloca��o foi bem sucedida.
        if (p) {
            p->n = n; // Tamanho do problema.
            p->h = h; // Tamanho m�ximo da pilha.
            p->i_caixas = 0; // Estas vari�veis controlam quantas caixas e quantos
            p->i_v = 0;      // valores foram efetivamente adicionados ao problema.

            // Aloca espa�o em mem�ria para o array de caixas.
            p->caixas = (caixa*) malloc(sizeof(caixa) * n);

            // Verifica se a aloca��o foi bem sucedida.
            if (!p->caixas) {
                free(p); // Libera a mem�ria alocada.
                return NULL; // Retorna NULL, devido a n�o ter conseguido alocar mem�ria.

            } else {
                // Aloca mem�ria para o array de valores (v) das caixas.
                p->v = (int*) malloc(sizeof(int) * n);

                // Verifica se a aloca��o foi bem sucedida.
                if (!p->v) {

                    free(p->caixas); // Libera a mem�ria do array de caixas.
                    free(p); // Libera a mem�ria do problema.
                    p = NULL; // O problema recebe NULL.

                }

            }
        }
    }

    return p; // Retorna uma inst�ncia do TAD problema ou NULL.
} // fim da fun��o cria_problema


/* Esta fun��o libera o espa�o alocado para uma inst�ncia do TAD problema.
 * O par�metro de entrada � uma vari�vel do tipo problema, cujo espa�o em
 * mem�ria ser� liberado. Nenhum valor � retornado pela fun��o.
 *
 * Complexidade: O(1)
 */
void termina_problema(problema p)
{
    if (p) {
        free(p->caixas); // Desaloca o vetor caixas.
        free(p->v); // Desaloca o vetor v.
        free(p); // Desaloca o espa�o utilizado pela vari�vel do tipo problema.
    }
} // fim da fun��o termina_problema


/* Adiciona uma caixa � inst�ncia do TAD problema. Os par�metros de entrada s�o:
 *
 *      - a vari�vel do tipo problema;
 *      - a caixa a ser adicionada a este problema.
 *
 * Retorna 1 (um), indicando sucesso na opera��o, ou retorna 0 (zero), caso n�o
 * haja mais espa�o para inser��es.
 *
 * Complexidade: O(1)
 */
int add_caixa(problema p, caixa c)
{
    // A vari�vel i_caixas controla quantas caixas de fato foram adicionadas e
    // n � o tamanho (n�mero de caixas) do problema.
    if (p->i_caixas < p->n) {
        p->caixas[p->i_caixas++] = c;   // Adiciona a caixa (c) ao vetor de caixas (p->caixas)
                                        // e incrementa i_caixas.
        return 1; // Sucesso na inser��o.
    }

    return 0; // Falha na inser��o.
} // fim da fun��o add_caixa


/* Adiciona um valor � inst�ncia do TAD problema. Os par�metros de entrada s�o:
 *
 *      - a vari�vel do tipo problema;
 *      - o valor a ser adicionado a este problema.
 *
 * Retorna 1 (um), indicando sucesso na opera��o, ou retorna 0 (zero), caso n�o
 * haja mais espa�o para inser��es.
 *
 * Complexidade: O(1)
 */
int add_valor(problema p, int v)
{
    // A vari�vel i_v controla quantas valores de fato foram adicionados e
    // n � o tamanho (n�mero de caixas) do problema.
    if (p->i_v < p->n) {
        p->v[p->i_v++] = v; // Adiciona o valor (v) ao vetor de valores (p->v) e incrementa i_v.
        return 1; // Sucesso na inser��o.
    }

    return 0; // Falha na inser��o.
} // fim da fun��o add_valor


/* Encontra a solu��o �tima para o problema de empilhamento de caixas, utilizando programa��o
 * din�mica e uma estrat�gia de resolu��o iterativa (bottom up). O par�metro de entrada p
 * cont�m todas as informa��es necess�rias � resolu��o do problema: a altura m�xima da pilha
 * (h), o conjunto n de caixas (com as respectivas dimens�es) e os valores associados a elas.
 * O par�metro tam, passado por refer�ncia, receber� o tamanho do vetor criado nesta fun��o.
 *
 * Retorna o valor da solu��o �tima encontrada, al�m de retornar um vetor (e seu tamanho) com
 * os �ndices das caixas utilizadas no empilhamento de altura m�xima h. Em caso de falha na
 * aloca��o de recursos computacionais, a fun��o retorna o valor -1, NULL no lugar do vetor
 * com os �ndices das caixas e o tamanho 0 (zero).
 *
 * Obs.: A fun��o cria e retorna um vetor alocado dinamicamente. � responsabilidade do utilizador
 * desta fun��o desaloc�-lo posteriormente.
 *
 * Complexidade: O(n�h)
 */
int empilhamento_bottom_up(problema p, int** vetor_de_caixas_empilhadas, int* tam)
{
    // As matrizes a seguir s�o utilizadas para o c�lculo da solu��o �tima e para o c�lculo
    // da sequ�ncia de caixas utilizadas no empilhamento.
    int **matriz_emp, **matriz_indices;

    int k, i, j, h_aux, solucao_otima, pos; // Vari�veis auxiliares.

    // Vetor, de listas encadeadas simples, utilizado para armazenar, para cada caixa, uma
    // rela��o das demais caixas que s�o empilh�veis sobre ela (empilhamento est�vel).
    lista *vetor_lista_de_caixas_empilhaveis;

    tipo_nodo *no; // Vari�vel auxiliar utilizada para percorrer a lista de caixas empilh�veis.

    // Verifica se a vari�vel passada como par�metro representa mem�ria alocada.
    if (*vetor_de_caixas_empilhadas) 
        free(*vetor_de_caixas_empilhadas); // Libera a mem�ria alocada.

    *vetor_de_caixas_empilhadas = NULL; // Inicializa��o das vari�veis passadas por
    *tam = 0;                           // refer�ncia e que retornar�o valores tamb�m.

    matriz_emp = (int**) malloc(sizeof(int*) * (p->h + 1)); // Aloca as linhas da matriz.
    if (!matriz_emp) // Verifica se a aloca��o foi bem sucedida.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.

    // Aloca o restante da matriz, sem fragmenta��o.
    matriz_emp[0] = (int*) malloc(sizeof(int) * (p->h + 1) * (p->n)); 
    if (!matriz_emp[0]) { // Verifica se a aloca��o foi bem sucedida.
        free(matriz_emp);       // Libera os recursos alocados anteriormente.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.
    }

    matriz_indices = (int**) malloc(sizeof(int*) * (p->h + 1)); // Aloca as linhas da matriz.
    if (!matriz_indices) { // Verifica se a aloca��o foi bem sucedida.
        free(matriz_emp[0]);    // Libera os recursos
        free(matriz_emp);       // alocados anteriormente.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.
    }

    // Aloca o restante da matriz, sem fragmenta��o.
    matriz_indices[0] = (int*) malloc(sizeof(int) * (p->h + 1) * p->n); 
    if (!matriz_indices[0]) { // Verifica se a aloca��o foi bem sucedida.
        free(matriz_emp[0]);    // Libera os recursos
        free(matriz_emp);       // alocados
        free(matriz_indices);   // anteriormente.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.
    }

    // Cria o vetor com as listas de caixas empilh�veis.
    vetor_lista_de_caixas_empilhaveis = cria_vetor_lista_de_caixas_empilhaveis(p->caixas, p->n);
    if (!vetor_lista_de_caixas_empilhaveis) { // Verifica se a aloca��o foi bem sucedida.
        free(matriz_emp[0]);     // Libera os
        free(matriz_emp);        // recursos
        free(matriz_indices[0]); // alocados
        free(matriz_indices);    // anteriormente.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.
    }

    // Inicializa��o de valores.
    for(i = 0; i < p->n; i++) {
        matriz_emp[0][i] = 0; // Inicializa a primeira linha da matriz com 0 (zero).
        matriz_indices[0][i] = -1;  // Inicializa a primeira linha da matriz que guarda
                                    // a sequ�ncia de caixas utilizadas no empilhamento
                                    // com o valor -1. Este valor indica que n�o houve
                                    // empilhamento de caixas naquela posi��o.
    }

    for(k = 1; k <= p->h; k++) { // Percorre todos os valores de 1 at� h (altura m�xima da pilha).

        matriz_emp[k] = matriz_emp[(k - 1)] + p->n;         // Corrige a refer�ncia de ponteiros da aloca��o
        matriz_indices[k] = matriz_indices[(k - 1)] + p->n; // din�mica de matriz sem fragmenta��o.

        for(i = 0; i < p->n; i++) { // Percorre todas as n caixas.

            matriz_emp[k][i] = 0;       // Inicializa a posi��o da matriz de solu��es �timas com 0 (zero).
            matriz_indices[k][i] = -1;  // Inicializa a posi��o da matriz da seq. de empilhamento com -1.

            h_aux = k - p->caixas[i].a; // Subtrai a altura da caixa da altura k da itera��o.

            if (h_aux >= 0) { // Se o valor anterior for n�o-negativo, a caixa cabe na pilha.

                // A seguir, percorre-se a lista de caixas que s�o empilh�veis a esta caixa
                // e verifica-se qual o empilhamento, de uma solu��o de subproblema menor,
                // mais o valor desta caixa apresenta o maior valor.
                no = vetor_lista_de_caixas_empilhaveis[i]->first;

                // As linhas comentadas abaixo s�o a forma de se percorrer a lista encadeada utilizando
                // opera��o do TAD lista (pr�pria para este fim).
                //no = NULL;
                //j = get_next_chave(vetor_lista_de_caixas_empilhaveis[i], &no);

                while(no) {
                    j = no->item.chave; // Cada n� da lista cont�m o �ndice de uma caixa que � empilh�vel.

                    // Atrav�s da estrutura condicional a seguir, escolhe-se o maior valor dentre todos
                    // os empilhamentos poss�veis. Ao final, a posi��o da matriz armazenar� a solu��o
                    // �tima para o conjunto (h, l, p), onde h � representado pelo �ndice das linhas da
                    // matriz de solu��es �timas (matriz_emp) e as dimens�es l e p est�o associadas ao
                    // �ndice das colunas, j� que ele est� associado a uma caixa com dimens�es (l e p).
                    //
                    // Ao mesmo tempo em que se calculam as solu��es �timas, a matriz com as sequ�ncias de
                    // empilhamentos (matriz_indices) tamb�m � gerada.
                    if ((p->v[i] + matriz_emp[h_aux][j]) > matriz_emp[k][i]) {
                        matriz_emp[k][i] = p->v[i] + matriz_emp[h_aux][j];
                        if (matriz_emp[h_aux][j] != 0) // O valor da c�lula (-1) s� � substitu�do
                            matriz_indices[k][i] = j;  // se a caixa for empilhada com outra(s).
                    }

                    no = no->next; // Altera a refer�ncia do ponteiro para o pr�ximo n� da lista.

                    // A linha comentada abaixo � a forma de se percorrer a lista encadeada utilizando
                    // opera��o do TAD lista (pr�pria para este fim).
                    //j = get_next_chave(vetor_lista_de_caixas_empilhaveis[i], &no);
                }
            }
        }
    }

    // Libera a mem�ria alocada para o vetor com as listas de caixas empilh�veis.
    termina_vetor_lista_de_caixas_empilhaveis(vetor_lista_de_caixas_empilhaveis, p->n);

    // A solu��o �tima, para uma altura m�xima h, � dada pelo valor m�ximo de todas
    // as solu��es (h, l, p), onde h � a �ltima linha da matriz.
    solucao_otima = max(matriz_emp[p->h], p->n, &pos);

    free(matriz_emp[0]);    // Libera a mem�ria alocada para a matriz
    free(matriz_emp);       // de solu��es �timas.

    // Gera um vetor com os �ndices das caixas utilizadas no empilhamento de altura m�xima h.
    (*vetor_de_caixas_empilhadas) = cria_vetor_caixas_empilhadas(matriz_indices, p->caixas, p->h, pos, tam);

    if (!(*vetor_de_caixas_empilhadas)) // Verifica se a aloca��o foi bem sucedida.
        solucao_otima = -1; // Falha, devido � aus�ncia de recursos computacionais.

    free(matriz_indices[0]);    // Libera a mem�ria alocada para a matriz
    free(matriz_indices);       // que guarda a sequ�ncia de empilhamento.

    return solucao_otima; // Retorna a solu��o �tima ou -1, em caso de falha na aloca��o de mem�ria.
} // fim da fun��o empilhamento_bottom_up


/* Fun��o recursiva que encontra uma solu��o �tima para o problema de empilhamento associado
 * a uma altura m�xima de pilha, largura e profundidade espec�ficas (h, l, p). O par�metro de
 * entrada h � a altura m�xima da pilha para a qual se deseja calcular a solu��o. O indice_caixa,
 * atrav�s da caixa que referencia, representa as dimens�es largura e profundidade (l e p) para
 * as quais esta solu��o � �tima.
 *
 * Os demais par�metros de entrada s�o:
 *
 *      - o tamanho do problema n, ou seja, o n�mero de caixas;
 *      - o vetor de caixas;
 *      - o vetor de valores associados �s caixas;
 *      - a matriz (h x n) que ser� usada para armazenar as diversas solu��es �timas, que � um artif�cio
 *           da programa��o din�mica para evitar c�lculos j� realizados. Na primeira chamada a esta
 *           fun��o recursiva, a matriz deve ter sua primeira linha inicilizada com zeros e as demais
 *           c�lulas com o valor -1 (o valor menos um indica que aquela posi��o da matriz ainda n�o
 *           foi calculada);
 *      - a matriz (h x n) que ser� utilizada para armazenar os �ndices das caixas que comp�em as sequ�ncias
 *           de empilhamentos. Na primeira chamada a esta fun��o recursiva, a matriz deve ter sua primeira
 *           linha inicializada com o valor -1;
 *      - um vetor com as listas de todas as caixas que s�o empilh�veis.
 *
 * Retorna o valor da solu��o �tima, para uma pilha de altura m�xima h, associada a uma largura e profundidade
 * espec�ficas da caixa de �ndice indice_caixa.
 *
 * Complexidade: O(nh)
 */
int empilhamento_recursiva(int h, int indice_caixa, int n, caixa* caixas, int* valores,
                           int** matriz_emp, int** matriz_indices, lista* vetor_lista_de_caixas_empilhaveis)
{
    int i, h_aux, emp_aux; // Vari�veis auxiliares.
    tipo_nodo *no; // Vari�vel do tipo tipo_nodo utilizada para percorrer a lista de caixas empilh�veis.

    if (matriz_emp[h][indice_caixa] != -1)  // Se o valor procurado j� foi calculado, ou seja, j� se
        return matriz_emp[h][indice_caixa]; // encontra na matriz de solu��es �timas, basta retorn�-lo.
    else {

        matriz_emp[h][indice_caixa] = 0;       // Inicializa a posi��o da matriz de solu��es �timas com 0 (zero).
        matriz_indices[h][indice_caixa] = -1;  // Inicializa a posi��o da matriz da seq. de empilhamento com -1.

        h_aux = h - caixas[indice_caixa].a; // Subtrai a altura da caixa da altura h (altura da solu��o procurada).

        if (h_aux >= 0) { // Se o valor anterior for n�o-negativo, a caixa cabe na pilha.

            // A seguir, percorre-se a lista de caixas que s�o empilh�veis � caixa de �ndice indice_caixa
            // e verifica-se qual o empilhamento, de uma solu��o de subproblema menor, mais o valor desta
            // caixa apresenta o maior valor.
            no = vetor_lista_de_caixas_empilhaveis[indice_caixa]->first;

            // As linhas comentadas abaixo s�o a forma de se percorrer a lista encadeada utilizando
            // opera��o do TAD lista (pr�pria para este fim).
            //no = NULL;
            //j = get_next_chave(vetor_lista_de_caixas_empilhaveis[indice_caixa], &no);

            while(no) {
                i = no->item.chave; // Cada n� da lista cont�m o �ndice de uma caixa que � empilh�vel.

                // A chamada recursiva a seguir � respons�vel por calcular a solu��o �tima de um subproblema
                // menor. Ao somar o resultado com o valor da caixa de �ndice indice_caixa, obt�m-se uma solu��o
                // de empilhamento, mas que n�o se sabe ser a �tima.
                emp_aux = empilhamento_recursiva(h_aux, i, n, caixas,
                                                 valores, matriz_emp, matriz_indices, vetor_lista_de_caixas_empilhaveis)
                          + valores[indice_caixa];

                // Atrav�s da estrutura condicional a seguir, escolhe-se o maior valor dentre todos
                // os empilhamentos poss�veis. Ao final, a posi��o da matriz armazenar� a solu��o
                // �tima para o conjunto (h, l, p), onde h � representado pelo �ndice das linhas da
                // matriz de solu��es �timas (matriz_emp) e as dimens�es l e p est�o associadas ao
                // �ndice das colunas, j� que ele est� associado a uma caixa com dimens�es (l e p).
                //
                // Ao mesmo tempo em que se calculam as solu��es �timas, a matriz com as sequ�ncias de
                // empilhamentos (matriz_indices) tamb�m � gerada.
                if (emp_aux > matriz_emp[h][indice_caixa]) {
                    matriz_emp[h][indice_caixa] = emp_aux;
                    if (emp_aux > valores[indice_caixa])        // O valor da c�lula (-1) s� � substitu�do
                        matriz_indices[h][indice_caixa] = i;    // se a caixa for empilhada com outra(s).
                }

                no = no->next; // Altera a refer�ncia do ponteiro para o pr�ximo n� da lista.

                // A linha comentada abaixo � a forma de se percorrer a lista encadeada utilizando
                // opera��o do TAD lista (pr�pria para este fim).
                //j = get_next_chave(vetor_lista_de_caixas_empilhaveis[indice_caixa], &no);
            }
        }
    }

    return matriz_emp[h][indice_caixa]; // Retorna a solu��o �tima, para uma altura h, l e p espec�ficas.
} // fim da fun��o empilhamento_recursiva


/* Encontra a solu��o �tima para o problema de empilhamento de caixas, utilizando programa��o
 * din�mica e uma estrat�gia de resolu��o recursiva (top down). O par�metro de entrada p
 * cont�m todas as informa��es necess�rias � resolu��o do problema: a altura m�xima da pilha
 * (h), o conjunto n de caixas (com as respectivas dimens�es) e os valores associados a elas.
 * O par�metro tam, passado por refer�ncia, receber� o tamanho do vetor criado nesta fun��o.
 *
 * Retorna o valor da solu��o �tima encontrada, al�m de retornar um vetor (e seu tamanho) com
 * os �ndices das caixas utilizadas no empilhamento de altura m�xima h. Em caso de falha na
 * aloca��o de recursos computacionais, a fun��o retorna o valor -1.
 *
 * Obs.: A fun��o cria e retorna um vetor alocado dinamicamente. � responsabilidade do utilizador
 * desta fun��o desaloc�-lo posteriormente.
 *
 * Complexidade: O(n�h)
 */
int empilhamento_top_down(problema p, int** vetor_de_caixas_empilhadas, int* tam)
{
    // As matrizes a seguir s�o utilizadas para o c�lculo da solu��o �tima e para o c�lculo
    // da sequ�ncia de caixas utilizadas no empilhamento.
    int **matriz_emp, **matriz_indices;
    int i, j, solucao_otima, pos;

    // Vetor, de listas encadeadas simples, utilizado para armazenar, para cada caixa, uma
    // rela��o das demais caixas que s�o empilh�veis sobre ela (empilhamento est�vel).
    lista *vetor_lista_de_caixas_empilhaveis;

    // Verifica se a vari�vel passada como par�metro representa mem�ria alocada.
    if (*vetor_de_caixas_empilhadas) 
        free(*vetor_de_caixas_empilhadas); // Libera a mem�ria alocada.

    *vetor_de_caixas_empilhadas = NULL; // Inicializa��o das vari�veis passadas por
    *tam = 0;                           // refer�ncia e que retornar�o valores tamb�m.

    matriz_emp = (int**) malloc(sizeof(int*) * (p->h + 1)); // Aloca as linhas da matriz.
    if (!matriz_emp) // Verifica se a aloca��o foi bem sucedida.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.

    // Aloca o restante da matriz, sem fragmenta��o.
    matriz_emp[0] = (int*) malloc(sizeof(int) * (p->h + 1) * (p->n)); 
    if (!matriz_emp[0]) { // Verifica se a aloca��o foi bem sucedida.
        free(matriz_emp); // Libera os recursos alocados anteriormente.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.
    }

    matriz_indices = (int**) malloc(sizeof(int*) * (p->h + 1)); // Aloca as linhas da matriz.
    if (!matriz_indices) { // Verifica se a aloca��o foi bem sucedida.
        free(matriz_emp); // Libera os recursos alocados anteriormente.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.
    }

    // Aloca o restante da matriz, sem fragmenta��o.
    matriz_indices[0] = (int*) malloc(sizeof(int) * (p->h + 1) * p->n); 
    if (!matriz_indices[0]) { // Verifica se a aloca��o foi bem sucedida.
        free(matriz_emp[0]);    // Libera os recursos
        free(matriz_emp);       // alocados
        free(matriz_indices);   // anteriormente.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.
    }

    // Cria o vetor com as listas de caixas empilh�veis.
    vetor_lista_de_caixas_empilhaveis = cria_vetor_lista_de_caixas_empilhaveis(p->caixas, p->n);
    if (!vetor_lista_de_caixas_empilhaveis) { // Verifica se a aloca��o foi bem sucedida.
        free(matriz_emp[0]);     // Libera os
        free(matriz_emp);        // recursos
        free(matriz_indices[0]); // alocados
        free(matriz_indices);    // anteriormente.
        return -1; // Falha no c�lculo da solu��o �tima, devido � aus�ncia de recursos computacionais.
    }

    // Inicializa��o de valores.
    for(i = 0; i < p->n; i++) {
        matriz_emp[0][i] = 0; // Inicializa a primeira linha da matriz com 0 (zero).
        matriz_indices[0][i] = -1;  // Inicializa a primeira linha da matriz que guarda
                                    // a sequ�ncia de caixas utilizadas no empilhamento
                                    // com o valor -1. Este valor indica que n�o houve
                                    // empilhamento de caixas naquela posi��o.
    }

    for(i = 1; i <= p->h; i++) { // Percorre todos os valores de 1 at� h (altura m�xima da pilha).

        matriz_emp[i] = matriz_emp[(i - 1)] + p->n;         // Corrige a refer�ncia de ponteiros da aloca��o
        matriz_indices[i] = matriz_indices[(i - 1)] + p->n; // din�mica de matriz sem fragmenta��o.

        // A seguir todas as c�lulas da linha, de ambas as matrizes, recebem o valor -1,
        // que indica para a fun��o recursiva que uma determinada posi��o da matriz de
        // solu��es �timas ainda n�o foi calculada.
        for(j = 0; j < p->n; j++) {
            matriz_emp[i][j] = -1;
            matriz_indices[i][j] = -1;
        }
    }

    // Percorrem-se as n caixas, calculando, atrav�s da fun��o recursiva, qual � a solu��o �tima
    // para a altura h. A solu��o �tima, para uma pilha de altura m�xima h, ser� o m�ximo de todos
    // esses n c�lculos.
    for(i = 0; i < p->n; i++) {
        // A fun��o empilhamento_recursiva calcula a solu��o �tima para uma pilha de altura m�xima
        // h e uma largura e profundidade (l e p) que est�o associadas a uma caixa de �ndice i.
        empilhamento_recursiva(p->h, i, p->n, p->caixas, p->v, matriz_emp, matriz_indices, vetor_lista_de_caixas_empilhaveis);
    }

    // A libera��o do vetor de listas � realizada atrav�s da fun��o espec�fica a seguir.
    termina_vetor_lista_de_caixas_empilhaveis(vetor_lista_de_caixas_empilhaveis, p->n);

    // A solu��o �tima ser� o m�ximo de todos os valores armazenados na linha de �ndice h
    // da matriz de solu��es �timas.
    solucao_otima = max(matriz_emp[p->h], p->n, &pos);

    free(matriz_emp[0]);    // Libera a mem�ria alocada para a matriz
    free(matriz_emp);       // de solu��es �timas.

    // Gera um vetor com os �ndices das caixas utilizadas no empilhamento de altura m�xima h.
    (*vetor_de_caixas_empilhadas) = cria_vetor_caixas_empilhadas(matriz_indices, p->caixas, p->h, pos, tam);

    if (!(*vetor_de_caixas_empilhadas)) // Verifica se a aloca��o foi bem sucedida.
        solucao_otima = -1; // Falha, devido � aus�ncia de recursos computacionais.

    free(matriz_indices[0]);    // Libera a mem�ria alocada para a matriz
    free(matriz_indices);       // que guarda a sequ�ncia de empilhamento.

    return solucao_otima; // Retorna a solu��o �tima ou -1, em caso de falha na aloca��o de mem�ria.
} // fim da fun��o empilhamento_top_down

/*********************************************************************************************************
 *                                  IN�CIO DAS FUN��ES INTERNAS
 *********************************************************************************************************/

/* Cria, a partir de uma matriz de �ndices, um vetor com a sequ�ncia de caixas utilizadas
 * em um empilhamento. A primeira posi��o do vetor cont�m o �ndice da caixa que � a base da
 * pilha e a �ltima posi��o cont�m o �ndice da caixa que est� no topo.
 *
 * Os par�metros de entrada s�o:
 *
 *      - a matriz que guarda todas as sequ�ncias de empilhamentos;
 *      - o vetor de caixas (para obten��o das alturas das caixas);
 *      - a altura h do empilhamento;
 *      - a posi��o da �ltima caixa empilhada;
 *      - uma vari�vel, passada por refer�ncia, para armazenar o tamanho do vetor criado.
 *
 * Retorna um vetor e o seu tamanho. Caso haja falha na aloca��o de recursos computacionais,
 * a fun��o retorna NULL e o tamanho do vetor como 0 (zero).
 *
 * Obs.: A fun��o cria e retorna um vetor alocado dinamicamente. � responsabilidade do utilizador
 * desta fun��o desaloc�-lo posteriormente.
 *
 * Complexidade: O(h)
 */
int* cria_vetor_caixas_empilhadas(int** matriz_indices, caixa* caixas, int h, int pos_ultima_caixa_empilhada, int* tam)
{
    int *vetor = NULL, i, aux; // Vetor que ser� retornado pela fun��o e vari�veis auxiliares.
    lista l; // Vari�vel auxiliar usada para criar uma lista encadeada.
    tipo_elemento e; // Vari�vel auxiliar usada para inser��o de elementos em lista encadeada.
    tipo_nodo *no; // Vari�vel auxiliar usada para percorrer os n�s de uma lista encadeada.

    *tam = 0; // O tamanho do vetor que ser� criado � inicializado com 0 (zero).

    // Cria-se uma lista encadeada simples para armazenar a sequ�ncia de caixas empilhadas.
    // A lista � necess�ria porque n�o se sabe o n�mero de caixas utilizadas.
    l = cria_lista();

    if (l) { // Verifica se a lista foi alocada com sucesso.

        e.chave = pos_ultima_caixa_empilhada;   // Insere, na lista, o �ndice da �ltima
        insere_apos(l, e);                      // caixa empilhada (a base da pilha).

        // O valor -1, na matriz de sequ�ncia de empilhamentos, indica que, naquela posi��o,
        // n�o houve empilhamento de uma caixa sobre outra(s).
        while (matriz_indices[h][pos_ultima_caixa_empilhada] != -1) {
            aux = matriz_indices[h][pos_ultima_caixa_empilhada];

            // Desconta, da altura h, a altura da caixa que j� foi inserida na lista.
            h = h - caixas[pos_ultima_caixa_empilhada].a;

            // A seguir, pega-se o �ndice da caixa que est� sobre a caixa que acabou de ser inserida na lista.
            pos_ultima_caixa_empilhada = aux; // Pega o valor da matriz, antes da altera��o do valor de h.

            e.chave = pos_ultima_caixa_empilhada;   // Insere, no fim da lista,
            insere_apos(l, e);                      // esta caixa.
        }

        // Aloca, dinamicamente, um vetor do tamanho da lista encadeada.
        vetor = (int*) malloc(sizeof(int) * tamanho_lista(l));
        *tam = tamanho_lista(l); // O tamanho do vetor � o tamanho da lista.

        // A seguir, percorrem-se todos os n�s da lista e copiam-se os �ndices das
        // caixas para o vetor, na mesma ordem em que eles aparecem na lista.
        no = l->first;
        i = 0;
        while(no) {
            vetor[i++] = no->item.chave; // O �ndice da caixa � copiado da lista para o vetor.
            no = no->next;
        }

        termina_lista(l); // Libera a mem�ria alocada para a lista.
    }

    return vetor; // Retorna o vetor criado ou NULL.
} // fim da fun��o cria_vetor_caixas_empilhadas


/* Cria, a partir do vetor de caixas (de tamanho n), um vetor com as listas de todas as caixas
 * que s�o empilh�veis sobre cada caixa. A primeira posi��o do vetor cont�m uma lista de todas
 * as caixas que s�o empilh�veis sobre a caixa de n�mero 1 (um), a segunda posi��o cont�m uma
 * lista de todas as caixas que s�o empilh�veis sobre a caixa de n�mero 2 e assim por diante.
 * Uma caixa � empilh�vel sobre outra se ela forma uma pilha est�vel, onde a largura � menor ou
 * igual � da caixa de baixo e, tamb�m, a profundidade � menor ou igual � da caixa de baixo.
 *
 * Os par�metros de entrada s�o:
 *
 *      - o vetor de caixas;
 *      - o tamanho n desse vetor.
 *
 * Retorna um vetor com todas as listas de caixas empilh�veis. Caso haja falha na aloca��o de
 * recursos computacionais, a fun��o retorna NULL.
 *
 * Obs.: A fun��o cria e retorna um vetor alocado dinamicamente e cada posi��o desse vetor tamb�m
 * cont�m uma lista alocada dinamicamente. � responsabilidade do utilizador desta fun��o desalocar,
 * posteriormente, cada uma das listas de cada posi��o do vetor e depois desalocar o vetor. A fun��o
 * termina_vetor_lista_de_caixas_empilhaveis automatiza este processo.
 *
 * Complexidade: O(n�)
 */
lista* cria_vetor_lista_de_caixas_empilhaveis(caixa* caixas, int n)
{
    lista *vetor_adj = NULL; // Vetor que ser� retornado pela fun��o.
    tipo_elemento e; // Vari�vel auxiliar usada para inser��o de elementos em lista encadeada.
    lista l; // Vari�vel auxiliar usada para cria��o das listas que compor�o o vetor de listas.
    int i, j; // Vari�veis usadas no controle de itera��es das estruturas de repeti��o.

    vetor_adj = (lista*) malloc(sizeof(lista) * n); // Aloca o vetor, de tamanho n.

    if (vetor_adj) { // Verifica se a aloca��o foi bem sucedida.

        for(i = 0; i < n; i++) { // Itera sobre o n�mero de caixas (n).

            l = cria_lista(); // Para cada caixa, cria-se uma lista encadeada.

            if (!l) { // Verifica se a aloca��o foi realizada.
                // Em caso de falha, libera os recursos que j� tenham sido alocados.
                termina_vetor_lista_de_caixas_empilhaveis(vetor_adj, i);
                return NULL; // Com a falha na aloca��o de recursos, retorna NULL.
            }

            vetor_adj[i] = l; // Atribui a lista criada a uma posi��o do vetor.

            // No la�o a seguir, percorrem-se todas as n caixas, verificando-se quais
            // s�o empilh�veis sobre a caixa de �ndice i e inserem-se seus �ndices (j)
            // na lista da posi��o i do vetor de listas de caixas empilh�veis.
            for(j = 0; j < n; j++) { // Itera sobre o n�mero de caixas (n).
                // Verifica se a caixa de �ndice j � empilh�vel sobre a caixa de �ndice i.
                if ((caixas[i].l >= caixas[j].l) && (caixas[i].p >= caixas[j].p)) {
                    e.chave = j;                    // Em caso positivo, insere o �ndice da
                    insere_apos(vetor_adj[i], e);   // caixa j na lista da caixa i.
                }
            }
        }
    }

    return vetor_adj; // Retorna o vetor de listas ou NULL.
} // fim da fun��o cria_vetor_lista_de_caixas_empilhaveis


/* Esta fun��o libera o espa�o alocado para um vetor de listas encadeadas.
 * Os par�metros de entrada s�o o vetor de listas encadeadas e seu tamanho
 * n. Nenhum valor � retornado pela fun��o.
 *
 * Complexidade: O(n�)
 */
void termina_vetor_lista_de_caixas_empilhaveis(lista* vetor, int n)
{
    int i;

    // Percorrem-se todas as posi��es do vetor e liberam-se os espa�os alocados
    // para cada uma das listas que o comp�em.
    for(i = 0; i < n; i++) {
        termina_lista(vetor[i]); // Desaloca a lista da posi��o i do vetor.
    }
    free(vetor); // Por fim, libera-se a mem�ria alocada para o vetor.
} // fim da fun��o termina_vetor_lista_de_caixas_empilhaveis

/* Calcula o valor m�ximo de um vetor de inteiros.
 *
 * Par�metros de entrada:
 *
 *      - o vetor de inteiros a ser pesquisado;
 *      - o tamanho (n) do vetor;
 *      - um par�metro passado por refer�ncia, para armazenar a posi��o do
 *        vetor que cont�m o valor m�ximo.
 *
 * Retorna o valor m�ximo e em qual posi��o do vetor esse valor foi encontrado. Se
 * o vetor passado com par�metro for NULL, retorna 0 (zero) e a posi��o como -1.
 *
 * Complexidade: O(n)
 */
int max(int *v, int n, int* pos)
{
    int i, maior;

    // Verifica se o vetor n�o � NULL.
    if (!v) {
        *pos = -1; // A posi��o retornada ser� -1.
        return 0; // Retorna 0 (zero).
    }

    maior = v[0]; // Inicializa a vari�vel maior com a primeira posi��o do vetor.
    *pos = 0; // pos come�a com o �ndice 0 (zero).

    // Itera sobre o vetor para checar se h� alguma posi��o com valor maior do que
    // a primeira posi��o.
    for (i = 1; i < n; i++)
        // Caso seja maior, atualizam-se a vari�veis maior e pos.
        if (v[i] > maior) {
            maior = v[i];
            *pos = i;
        }

    return maior;
} // fim da fun��o max
