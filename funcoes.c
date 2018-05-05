#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

/* Processa o arquivo texto passado como parâmetro e retorna uma instância do tipo abstrato
 * de dados (TAD) problema (empilhamento de caixas). Os dados obtidos do arquivo texto são:
 *
 *      - tamanho do problema (número de caixas);
 *      - altura máxima da pilha de caixas;
 *      - vetor de caixas, com as suas respectivas medidas (largura, altura e profundidade);
 *      - vetor de valores associados às caixas.
 *
 * Retorna NULL caso haja problemas com a abertura do arquivo (arquivo inexistente, permissão
 * negada, etc.) ou caso haja problemas no parsing do arquivo texto (organização dos dados
 * diferente da forma especificada no enunciado do trabalho).
 */
problema processa_arquivo_entrada(const char* nome_arquivo)
{
    FILE *ptr_arq; // Ponteiro para o arquivo a ser manipulado.
    int n, h, i, aux, v; // Variáveis auxiliares.
    caixa c; // Variável auxiliar.
    problema p = NULL; // Instância do tipo problema que será retornada
    // ao fim do processamento do arquivo.

    // Abre o arquivo texto, como somente leitura.
    ptr_arq = fopen(nome_arquivo, "r");

    // Verificando se o arquivo foi aberto com sucesso.
    if (ptr_arq != NULL) {

        // Tenta realizar a leitura dos valores iniciais (n e h) do arquivo texto e
        // verifica se houve sucesso na leitura.
        if (fscanf(ptr_arq, "%d%d", &n, &h) == 2) {

            // Aloca memória para uma instância do TAD problema, com tamanho igual a 2*n,
            // já que cada caixa pode ser rotacionada de duas maneiras.
            p = cria_problema((2*n), h);

            // Verifica se a alocação de memória foi realizada com sucesso.
            if (!p) {

                fclose(ptr_arq); // Fecha o arquivo e libera o recurso.
                return p; // Retorna NULL.

            }

            // Realiza a leitura dos n valores associados a cada caixa e adiciona-os
            // à instância do TAD problema.
            for(i = 1; i <= n && !feof(ptr_arq); i++) {

                // Tenta ler um int do arquivo e verifica se a leitura foi bem sucedida.
                if (fscanf(ptr_arq, "%d", &v)) {

                    add_valor(p, v); // Adiciona o valor associado à caixa com a rotação 1.
                    add_valor(p, v); // Adiciona o valor associado à caixa com a rotação 2.

                } else {
                    // Caso haja problemas no parsing do arquivo texto, os recursos
                    // são liberados e retorna-se NULL.
                    termina_problema(p); // Libera a memória alocada.
                    fclose(ptr_arq); // Fecha o arquivo.
                    return NULL; // Retorna NULL devido a problemas no parsing do arquivo.
                }
            }

            // Realiza a leitura das dimensões das n caixas e adiciona-as
            // à instância do TAD problema.
            for(i = 1; i <= n && !feof(ptr_arq); i++) {

                // Tenta ler as dimensões da caixa e verifica se alitura foi bem sucedida.
                if (fscanf(ptr_arq, "%d", &c.l) &&      // Leitura da largura.
                        fscanf(ptr_arq, "%d", &c.a) &&  // Leitura da altura.
                        fscanf(ptr_arq, "%d", &c.p)) {  // Leitura da profundidade.

                    add_caixa(p, c); // Adiciona a caixa ao problema.

                    aux = c.l; // Rotaciona a caixa, ou seja,
                    c.l = c.a; // troca a sua medida de largura
                    c.a = aux; // com a altura.

                    add_caixa(p, c); // Adiciona a caixa (com rotação 2) ao problema.

                } else {
                    // Caso haja problemas no parsing do arquivo texto, os recursos
                    // são liberados e interrompe-se a estrutura de repetição.
                    termina_problema(p); // Libera a memória alocada.
                    p = NULL; // A variável recebe NULL, que será o valor retornado pela função.
                    break;  // Interrompe o for devido a problemas no parsing do arquivo.
                }
            }
        }
        fclose(ptr_arq); // Fecha o arquivo.
    }

    return p; // Retorna uma instância do TAD problema ou NULL.
} // fim da função processa_arquivo_entrada


/* Grava a solução do problema de empilhamento em um arquivo texto.
 *
 * Parâmetros de entrada:
 *
 *      - nome_arquivo: nome do arquivo a ser gravado;
 *      - solucao_otima: valor da solução ótima calculada;
 *      - vetor_de_caixas_empilhadas: array com os índices das caixas que formam a
 *                                    pilha da solução ótima;
 *      - tam: tamanho do array vetor_de_caixas_empilhadas.
 *
 * A função retorna 1 (um) em caso de sucesso na gravação do arquivo ou retorna 0 (zero),
 * caso contrário.
 */
int gera_arquivo_saida(const char* nome_arquivo, int solucao_otima, int* vetor_de_caixas_empilhadas, int tam)
{
    FILE *ptr_arq; // Ponteiro para o arquivo a ser manipulado.
    int i; // Variável auxiliar.

    // Cria um arquivo para gravação. Se o arquivo já existe, descarta o conteúdo atual.
    ptr_arq = fopen(nome_arquivo, "w");

    // Verificando se o arquivo foi criado com sucesso.
    if (ptr_arq != NULL) {

        // Grava o valor da solução ótima e o número de caixas que formam a pilha
        // desta solução.
        fprintf(ptr_arq, "%d\n%d\n", solucao_otima, tam);

        // Itera sobre o vetor_de_caixas_empilhadas para gravar o número da caixa e
        // a sua rotação, no arquivo de saída.
        for(i = 0; i < tam; i++) {

            // O primeiro valor calculado é o número da caixa (de acordo com a ordem
            // em que ela aparece no arquivo texto de entrada). O segundo valor corresponde
            // à rotação da caixa.
            fprintf(ptr_arq, "%d %d\n",
                    (vetor_de_caixas_empilhadas[i] % 2 == 0) ?      // Se o índice da caixa for par, soma-se
                    ((vetor_de_caixas_empilhadas[i] + 2) / 2) : // 2 e divide-se por 2. Caso contrário,
                    ((vetor_de_caixas_empilhadas[i] + 1) / 2),  // soma-se 1 e divide-se por 2.
                    (vetor_de_caixas_empilhadas[i] % 2 + 1)  // Índice par é igual a rotação 1. Ímpar é rotação 2.
                   );
        }

        fclose(ptr_arq); // Fecha o arquivo.

        return 1; // Sucesso na gravação do arquivo.
    }

    return 0; // Falha na gravação do arquivo.
} // fim da função gera_arquivo_saida
