#include <stdlib.h>
#include <stdio.h>
#include "lista_encadeada.h"

/* Aloca espaço em memória para uma instância do TAD lista. Não há parâmetros
 * de entrada.
 *
 * Retorna uma instância do TAD lista ou retorna NULL, caso haja problemas
 * na alocação dinâmica de memória.
 *
 * Complexidade: O(1)
 */
lista cria_lista()
{
    lista l = NULL; // Inicializa a variável.

    l = (lista) malloc(sizeof(tipo_lista)); // Aloca espaço em memória.

    if(l != NULL) { // Verifica se a alocação foi bem sucedida.
        l->first = NULL; // Inicializa os ponteiros que referenciam
        l->last = NULL;  // o primeiro e o último nós da lista com NULL.
        l->tamanho = 0;  // O tamanho da lista recém criada é 0 (zero).
    }

    return l; // Retorna a lista criada ou NULL.
} // fim da função cria_lista


/* Libera o espaço alocado para uma instância do TAD lista. O parâmetro de
 * entrada é uma variável do tipo lista, cujo espaço em memória será liberado.
 * Nenhum valor é retornado pela função.
 *
 * Complexidade: O(1)
 */
void termina_lista(lista l)
{
    tipo_nodo *p; // Variável auxiliar tipo_nodo usada para iterar sobre os nós da lista.

    // A estrutura de repetição a seguir, itera sobre toda a lista, a partir do primeiro nó.
    // Ao desalocar a memória utilizada pelo primeiro nó, a lista diminui sucessivamente, até
    // que seu primeiro nó também seja NULL.
    while(l->first != NULL) {
        p = l->first; // A variável p recebe a referência do primeiro nó da lista.
        l->first = l->first->next; // O primeiro nó da lista é alterado para o seu subsequente.
        free(p); // Libera o nó p (primeiro nó da lista).
    }
    free(l); // Desaloca a memória restante.
} // fim da função termina_lista


/* Retorna o tamanho de uma instância do TAD lista. O parâmetro de entrada é
 * uma variável do tipo lista.
 *
 * Complexidade: O(1)
 */
int tamanho_lista(lista l)
{
    return l->tamanho;
} // fim da função tamanho_lista


/* Insere um elemento, como um novo nó, ao final da lista. O parâmetro de entrada
 * é a lista em que a inserção será realizada e o elemento a ser inserido.
 *
 * Retorna 1 (um) em caso de sucesso ou 0 (zero), caso haja falha na alocação de
 * memória.
 *
 * Complexidade: O(1)
 */
int insere_apos(lista l, tipo_elemento e)
{
    tipo_nodo *p; // Variável auxiliar usada para alocar memória.

    p = (tipo_nodo*) malloc(sizeof(tipo_nodo)); // Aloca a memória necessária para o nó que será inserido.

    if (!p) // Verifica se alocação falhou.
        return 0; // Retorna 0 (zero), devido a problemas de alocação de memória.

    p->item = e; // O elemento passado como parâmetro é copiado para o nó.
    p->next = NULL; // Como o nó será o último da lista, a sua referência para o próximo é NULL.

    if (l->first == NULL) // Verifica se a lista está vazia.
        l->first = p; // Se está vazia, o nó criado será o primeiro.
    else
        l->last->next = p; // Se a lista não está vazia, o nó criado será o subsequente ao último nó.

    l->last = p; // O nó criado passa a ser o último nó da lista.
    l->tamanho++; // Incrementa o tamanho da lista.

    return 1; // Retorna 1 (um) indicando sucesso na inserção.
} // fim da função insere_apos


/* Insere um elemento, como um novo nó, no início da lista. O parâmetro de entrada
 * é a lista em que a inserção será realizada e o elemento a ser inserido.
 *
 * Retorna 1 (um) em caso de sucesso ou 0 (zero), caso haja falha na alocação de
 * memória.
 *
 * Complexidade: O(1)
 */
int insere_inicio(lista l, tipo_elemento e)
{
    tipo_nodo *p; // Variável auxiliar usada para alocar memória.

    p = (tipo_nodo*) malloc(sizeof(tipo_nodo)); // Aloca a memória necessária para o nó que será inserido.

    if (!p) // Verifica se alocação falhou.
        return 0; // Retorna 0 (zero), devido a problemas de alocação de memória.

    p->item = e; // O elemento passado como parâmetro é copiado para o nó.
    p->next = l->first; // Como o nó será o primeiro da lista, a sua referência para o próximo
    // é o nó que está como primeiro atualmente.
    l->tamanho++; // Incrementa o tamanho da lista.
    l->first = p; // O nó criado passa a ser o primeiro nó da lista.

    if (l->tamanho == 1) // Se a lista tem tamanho um, isso significa que o nó inserido
        l->last = p;     // também será o último nó da lista.

    return 1; // Retorna 1 (um) indicando sucesso na inserção.
} // fim da função insere_inicio


/* Permite obter o valor armazenado em um nó de uma lista, com um artifício que permite manter
 * a posição do nó e assim percorrer todos os nós sequencialmente. Os parâmetros de entrada são
 * a lista que se deseja consultar e a referência para o último nó lido. Se último nó lido é NULL,
 * retorna-se o conteúdo do primeiro nó da lista. Caso contrário, retorna-se o conteúdo do nó
 * subsequente ao último nó lido.
 *
 * Retorna o conteúdo de um nó da lista e retorna também a referência para o nó, através do parâmetro
 * de entrada tipo_nodo* passado por referência.
 *
 * Complexidade: O(1)
 */
tipo_chave get_next_chave(lista l, tipo_nodo** n)
{
    if ((*n) == NULL)       // Se o parâmetro é NULL o nó lido
        (*n) = l->first;    // será o primeiro nó da lista.
    else
        (*n) = (*n)->next; // Caso contrário, o nó lido será o nó subsequente ao último lido.

    if (*n)
        return (*n)->item.chave; // Retorna o conteúdo do nó.
    else
        return 0; // Retorna 0 (zero) em caso de não haver nó (NULL).
} // fim da função get_next_chave
