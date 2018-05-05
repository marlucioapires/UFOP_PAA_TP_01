#include <stdlib.h>
#include <stdio.h>
#include "lista_encadeada.h"

/* Aloca espa�o em mem�ria para uma inst�ncia do TAD lista. N�o h� par�metros
 * de entrada.
 *
 * Retorna uma inst�ncia do TAD lista ou retorna NULL, caso haja problemas
 * na aloca��o din�mica de mem�ria.
 *
 * Complexidade: O(1)
 */
lista cria_lista()
{
    lista l = NULL; // Inicializa a vari�vel.

    l = (lista) malloc(sizeof(tipo_lista)); // Aloca espa�o em mem�ria.

    if(l != NULL) { // Verifica se a aloca��o foi bem sucedida.
        l->first = NULL; // Inicializa os ponteiros que referenciam
        l->last = NULL;  // o primeiro e o �ltimo n�s da lista com NULL.
        l->tamanho = 0;  // O tamanho da lista rec�m criada � 0 (zero).
    }

    return l; // Retorna a lista criada ou NULL.
} // fim da fun��o cria_lista


/* Libera o espa�o alocado para uma inst�ncia do TAD lista. O par�metro de
 * entrada � uma vari�vel do tipo lista, cujo espa�o em mem�ria ser� liberado.
 * Nenhum valor � retornado pela fun��o.
 *
 * Complexidade: O(1)
 */
void termina_lista(lista l)
{
    tipo_nodo *p; // Vari�vel auxiliar tipo_nodo usada para iterar sobre os n�s da lista.

    // A estrutura de repeti��o a seguir, itera sobre toda a lista, a partir do primeiro n�.
    // Ao desalocar a mem�ria utilizada pelo primeiro n�, a lista diminui sucessivamente, at�
    // que seu primeiro n� tamb�m seja NULL.
    while(l->first != NULL) {
        p = l->first; // A vari�vel p recebe a refer�ncia do primeiro n� da lista.
        l->first = l->first->next; // O primeiro n� da lista � alterado para o seu subsequente.
        free(p); // Libera o n� p (primeiro n� da lista).
    }
    free(l); // Desaloca a mem�ria restante.
} // fim da fun��o termina_lista


/* Retorna o tamanho de uma inst�ncia do TAD lista. O par�metro de entrada �
 * uma vari�vel do tipo lista.
 *
 * Complexidade: O(1)
 */
int tamanho_lista(lista l)
{
    return l->tamanho;
} // fim da fun��o tamanho_lista


/* Insere um elemento, como um novo n�, ao final da lista. O par�metro de entrada
 * � a lista em que a inser��o ser� realizada e o elemento a ser inserido.
 *
 * Retorna 1 (um) em caso de sucesso ou 0 (zero), caso haja falha na aloca��o de
 * mem�ria.
 *
 * Complexidade: O(1)
 */
int insere_apos(lista l, tipo_elemento e)
{
    tipo_nodo *p; // Vari�vel auxiliar usada para alocar mem�ria.

    p = (tipo_nodo*) malloc(sizeof(tipo_nodo)); // Aloca a mem�ria necess�ria para o n� que ser� inserido.

    if (!p) // Verifica se aloca��o falhou.
        return 0; // Retorna 0 (zero), devido a problemas de aloca��o de mem�ria.

    p->item = e; // O elemento passado como par�metro � copiado para o n�.
    p->next = NULL; // Como o n� ser� o �ltimo da lista, a sua refer�ncia para o pr�ximo � NULL.

    if (l->first == NULL) // Verifica se a lista est� vazia.
        l->first = p; // Se est� vazia, o n� criado ser� o primeiro.
    else
        l->last->next = p; // Se a lista n�o est� vazia, o n� criado ser� o subsequente ao �ltimo n�.

    l->last = p; // O n� criado passa a ser o �ltimo n� da lista.
    l->tamanho++; // Incrementa o tamanho da lista.

    return 1; // Retorna 1 (um) indicando sucesso na inser��o.
} // fim da fun��o insere_apos


/* Insere um elemento, como um novo n�, no in�cio da lista. O par�metro de entrada
 * � a lista em que a inser��o ser� realizada e o elemento a ser inserido.
 *
 * Retorna 1 (um) em caso de sucesso ou 0 (zero), caso haja falha na aloca��o de
 * mem�ria.
 *
 * Complexidade: O(1)
 */
int insere_inicio(lista l, tipo_elemento e)
{
    tipo_nodo *p; // Vari�vel auxiliar usada para alocar mem�ria.

    p = (tipo_nodo*) malloc(sizeof(tipo_nodo)); // Aloca a mem�ria necess�ria para o n� que ser� inserido.

    if (!p) // Verifica se aloca��o falhou.
        return 0; // Retorna 0 (zero), devido a problemas de aloca��o de mem�ria.

    p->item = e; // O elemento passado como par�metro � copiado para o n�.
    p->next = l->first; // Como o n� ser� o primeiro da lista, a sua refer�ncia para o pr�ximo
                        // � o n� que est� como primeiro atualmente.
    l->tamanho++; // Incrementa o tamanho da lista.
    l->first = p; // O n� criado passa a ser o primeiro n� da lista.

    if (l->tamanho == 1) // Se a lista tem tamanho um, isso significa que o n� inserido
        l->last = p;     // tamb�m ser� o �ltimo n� da lista.

    return 1; // Retorna 1 (um) indicando sucesso na inser��o.
} // fim da fun��o insere_inicio


/* Permite obter o valor armazenado em um n� de uma lista, com um artif�cio que permite manter
 * a posi��o do n� e assim percorrer todos os n�s sequencialmente. Os par�metros de entrada s�o
 * a lista que se deseja consultar e a refer�ncia para o �ltimo n� lido. Se �ltimo n� lido � NULL,
 * retorna-se o conte�do do primeiro n� da lista. Caso contr�rio, retorna-se o conte�do do n�
 * subsequente ao �ltimo n� lido.
 *
 * Retorna o conte�do de um n� da lista e retorna tamb�m a refer�ncia para o n�, atrav�s do par�metro
 * de entrada tipo_nodo* passado por refer�ncia.
 *
 * Complexidade: O(1)
 */
tipo_chave get_next_chave(lista l, tipo_nodo** n)
{
    if ((*n) == NULL)       // Se o par�metro � NULL o n� lido
        (*n) = l->first;    // ser� o primeiro n� da lista.
    else
        (*n) = (*n)->next; // Caso contr�rio, o n� lido ser� o n� subsequente ao �ltimo lido.

    if (*n)
        return (*n)->item.chave; // Retorna o conte�do do n�.
    else
        return 0; // Retorna 0 (zero) em caso de n�o haver n� (NULL).
} // fim da fun��o get_next_chave
