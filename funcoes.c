#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

/* Processa o arquivo texto passado como par�metro e retorna uma inst�ncia do tipo abstrato
 * de dados (TAD) problema (empilhamento de caixas). Os dados obtidos do arquivo texto s�o:
 *
 *      - tamanho do problema (n�mero de caixas);
 *      - altura m�xima da pilha de caixas;
 *      - vetor de caixas, com as suas respectivas medidas (largura, altura e profundidade);
 *      - vetor de valores associados �s caixas.
 *
 * Retorna NULL caso haja problemas com a abertura do arquivo (arquivo inexistente, permiss�o
 * negada, etc.) ou caso haja problemas no parsing do arquivo texto (organiza��o dos dados
 * diferente da forma especificada no enuncSiado do trabalho).
 */
problema processa_arquivo_entrada(const char* nome_arquivo)
{
    FILE *ptr_arq; // Ponteiro para o arquivo a ser manipulado.
    int n, h, i, aux, v; // Vari�veis auxiliares.
    caixa c; // Vari�vel auxiliar.
    problema p = NULL; // Inst�ncia do tipo problema que ser� retornada
                       // ao fim do processamento do arquivo.

    // Abre o arquivo texto, como somente leitura.
    ptr_arq = fopen(nome_arquivo, "r");

    // Verificando se o arquivo foi aberto com sucesso.
    if (ptr_arq != NULL) {

        // Tenta realizar a leitura dos valores iniciais (n e h) do arquivo texto.
        if (fscanf(ptr_arq, "%d%d", &n, &h) == 2) {

            // Aloca mem�ria para uma inst�ncia do TAD problema, com tamanho igual a 2*n,
            // j� que cada caixa pode ser rotacionadade duas maneiras.
            p = cria_problema((2*n), h);

            // Verifica se a aloca��o de mem�ria foi realizada com sucesso.
            if (!p) {

                fclose(ptr_arq); // Fecha o arquivo e libera o recurso.
                return p; // Retorna NULL.

            }

            // Realiza a leitura dos n valores associados a cada caixa e adiciona-os
            // � inst�ncia do TAD problema.
            for(i = 1; i <= n && !feof(ptr_arq); i++) {

                if (fscanf(ptr_arq, "%d", &v)) {

                    add_valor(p, v); // Adiciona o valor associado � caixa com a rota��o 1.
                    add_valor(p, v); // Adiciona o valor associado � caixa com a rota��o 2.

                } else {
                    // Caso haja problemas no parsing do arquivo texto, os recursos
                    // s�o liberados e retorna-se NULL.
                    termina_problema(p);
                    fclose(ptr_arq);
                    return NULL;
                }
            }

            // Realiza a leitura das dimens�es das n caixas e adiciona-as
            // � inst�ncia do TAD problema.
            for(i = 1; i <= n && !feof(ptr_arq); i++) {

                if (fscanf(ptr_arq, "%d", &c.l) &&      // Leitura da largura.
                        fscanf(ptr_arq, "%d", &c.a) &&  // Leitura da altura.
                        fscanf(ptr_arq, "%d", &c.p)) {  // Leitura da profundidade.

                    add_caixa(p, c); // Adiciona a caixa ao problema.

                    aux = c.l; // Rotaciona a caixa, ou seja,
                    c.l = c.a; // troca a sua medida de largura
                    c.a = aux; // com a altura.

                    add_caixa(p, c); // Adiciona a caixa (com rota��o 2) ao problema.

                } else {
                    // Caso haja problemas no parsing do arquivo texto, os recursos
                    // s�o liberados e retorna-se NULL.
                    termina_problema(p);
                    p = NULL;
                    break;
                }
            }
        }
        fclose(ptr_arq); // Fecha o arquivo.
    }

    return p;
} // fim da fun��o processa_arquivo_entrada


/* Grava a solu��o do problema de empilhamento em um arquivo texto.
 *
 * Par�metros de entrada:
 *
 *      - nome_arquivo: nome do arquivo a ser gravado;
 *      - solucao_otima: valor da solu��o �tima calculada;
 *      - vetor_de_caixas_empilhadas: array com os �ndices das caixas que formam a
 *                                    pilha da solu��o �tima;
 *      - tam: tamanho do array vetor_de_caixas_empilhadas.
 *
 * A fun��o retorna 1 (um) em caso de sucesso na grava��o do arquivo ou retorna 0 (zero),
 * caso contr�rio.
 */
int gera_arquivo_saida(const char* nome_arquivo, int solucao_otima, int* vetor_de_caixas_empilhadas, int tam)
{
    FILE *ptr_arq; // Ponteiro para o arquivo a ser manipulado.
    int i; // Vari�vel auxiliar.

    // Cria um arquivo para grava��o. Se o arquivo j� existe, descarta o conte�do atual.
    ptr_arq = fopen(nome_arquivo, "w");

    // Verificando se o arquivo foi criado com sucesso.
    if (ptr_arq != NULL) {

        // Grava o valor da solu��o �tima e o n�mero de caixas que formam a pilha
        // desta solu��o.
        fprintf(ptr_arq, "%d\n%d\n", solucao_otima, tam);

        // Itera sobre o vetor_de_caixas_empilhadas para gravar o n�mero da caixa e
        // a sua rota��o, no arquivo de sa�da.
        for(i = 0; i < tam; i++) {

            // O primeiro valor calculado � o n�mero da caixa (de acordo com a ordem
            // em que ela aparece no arquivo texto de entrada). O segundo valor corresponde
            // � rota��o da caixa.
            fprintf(ptr_arq, "%d %d\n",
                    (vetor_de_caixas_empilhadas[i] % 2 == 0) ?      // Se o �ndice da caixa for par, soma-se
                        ((vetor_de_caixas_empilhadas[i] + 2) / 2) : // 2 e divide-se por 2. Caso contr�rio,
                        ((vetor_de_caixas_empilhadas[i] + 1) / 2),  // soma-se 1 e divide-se por 2.
                    (vetor_de_caixas_empilhadas[i] % 2 + 1)  // �ndice par � igual a rota��o 1. �mpar � rota��o 2.
                   );
        }

        fclose(ptr_arq); // Fecha o arquivo.

        return 1; // Sucesso na grava��o do arquivo.
    }

    return 0; // Falha na grava��o do arquivo.
} // fim da fun��o gera_arquivo_saida

