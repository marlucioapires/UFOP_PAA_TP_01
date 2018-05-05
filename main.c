/*
* Universidade Federal de Ouro Preto
* Departamento de Computação
* Curso de Mestrado Acadêmico em Ciência da Computação do Programa de Pós-Graduação em Ciência da Computação (PPGCC)
* Primeiro Trabalho Prático da Disciplina PCC104 - Projeto e Análise de Algoritmos
* Prof.: Dr. Haroldo Gambini Santos
*
* Objetivo do trabalho: Conforme especificação apresentada em sala, este trabalho tem por objetivo
* implementar (em linguagem C) uma solução, através de programação dinâmica, para o problema de construir
* uma pilha de altura máxima h, incluindo o maior valor possível em caixas, com repetição ilimitada.
*
* Autor: Marlúcio Alves Pires - matrícula 2018.10391.
* E-mail: marlucioapires@gmail.com
*
* Data da última modificação: 05 de maio de 2018.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

/* Função principal do programa. Os parâmetros de execução (localização e nomes dos arquivos
 * de entrada e de saída) devem ser passados via linha de comando, com a seguinte sintaxe:
 *
 * nome_executavel [-r|-R] <arquivo_entrada> <arquivo_saida>
 *
 * O parâmetro -r (ou -R) indica que se deseja realizar a solução do problema de empilhamento
 * através de uma função recursiva (top-down). Se o parâmetro -r (ou -R) é omitido, a solução
 * do problema se dará por uma função iterativa (bottom-up).
 *
 * A função faz o tratamento dos possíveis erros, emite mensagens ao usuário e retorna o código
 * 0 (zero), indicado para o sistema operacional que sua execução foi realizada com sucesso.
 *
 * Obs.: Para obter o tempo de execução do programa, favor descomentar os trechos de código que
 * estão suprimidos. O tempo é exibido no final da execução.
 */
int main(int argc, char* argv[])
{
    clock_t inicio = clock() /* Inicia a contagem do tempo. */, fim; // Variáveis para medição do tempo de execução.
    double segundos; // Variável auxiliar.

    // Variáveis usadas para guardar o vetor com as caixas usadas no empilhamento,
    // o tamanho do vetor e a solução ótima para o problema.
    int *vetor_de_caixas_empilhadas = NULL, tam, solucao;

    // Variáveis auxiliares, usadas no processamento dos parâmetros passados via linha de comando.
    int ind_arquivo_entrada = 1, recursivo = 0;

    // Variável do tipo abstrato de dados (TAD) problema.
    problema p = NULL;

    // A estrutura a seguir verifica se foram passados, no mínimo, 3 parâmetros (o nome do executável
    // e os nomes dos arquivos de entrada e saída) via linha de comando.
    if (argc < 3) {
        printf("\nERRO: Ausencia de parametros!\nExecucao encerrada.\n");   // Caso haja menos de 3 parâmetros
        return 0;                                                           // a execução é encerrada.
    }

    // A seguir verifica-se se o parâmetro opcional -r (ou -R) foi informado.
    if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-R") == 0) {
        if (argc < 4) { // Com o parâmetro -r (ou -R) deve haver 4 parâmetros.
            printf("\nERRO: Ausencia de parametros!\nExecucao encerrada.\n");   // Caso haja menos de 4 parâmetros
            return 0;                                                           // a execução é encerrada.
        }

        recursivo = 1;  // Se o parâmetro -r (ou -R) foi informado e há 4 parâmetros, no mínimo, a variável
        // recursivo recebe 1 (um), como indicativo de que se deseja executar a função
        // recursiva (top-down).
        ind_arquivo_entrada++; // O nome do arquivo de entrada passa a ser o parâmetro seguinte ao -r (ou -R).
    }

    // A seguir, o realiza-se o processamento do arquivo texto de entrada, para obtenção dos dados
    // do problema de empilhamento de caixas. Uma instância do tipo problema é preenchida com esses
    // dados e retornada para a variável p. Se houver falhas no processamento ou problemas de alocação
    // de memória o valor retornado pela função é NULL.
    p = processa_arquivo_entrada(argv[ind_arquivo_entrada]);

    if (p) { // Verifica se o processamento do arquivo foi bem sucedido.

        if (recursivo) // Analisa qual será a função usada na solução do problema.
            solucao = empilhamento_top_down(p, &vetor_de_caixas_empilhadas, &tam); // Função recursiva.
        else
            solucao = empilhamento_bottom_up(p, &vetor_de_caixas_empilhadas, &tam); // Função iterativa.

        termina_problema(p); // Desaloca a memória usada para armazenar os dados do problema.

        if (solucao != -1) { // Se a variável contém o valor -1, houve falha de alocação de memória.

            // A seguir, gera-se o arquivo de saída, conforme formato definido no enunciado do trabalho.
            if (!gera_arquivo_saida(argv[(ind_arquivo_entrada + 1)], solucao, vetor_de_caixas_empilhadas, tam)) {
                // Se houve erro na geração do arquivo de saída, exibe mensagem de erro para o usuário.
                printf("\nERRO: Problemas na gravação do arquivo \"%s\".\n", argv[(ind_arquivo_entrada + 1)]);
            }

            if (vetor_de_caixas_empilhadas)
                free(vetor_de_caixas_empilhadas); // Desaloca a memória usada para o vetor.
        } else {
            // Exibe mensagem de erro devido a falha na alocação de memória para resolução do problema.
            printf("\nERRO: Problemas na alocação de memória.\n");
        }
    } else {
        // Exibe mensagem de erro ocasionada no processamento do arquivo texto de entrada.
        printf("\nERRO: Problemas na abertura/leitura/processamento do arquivo \"%s\".\n", argv[ind_arquivo_entrada]);
    }

    fim = clock(); // Termina a contagem de tempo de execução do programa.
    segundos = ((double) fim - inicio) / ((double) CLOCKS_PER_SEC); // Converte o tempo para segundos.
    printf("\nTEMPO DE EXECUCAO: %.3f\n", segundos); // Exibe o tempo de execução do programa.
    return 0; // Código de retorno para o sistema operacional.
} // fim da função main
