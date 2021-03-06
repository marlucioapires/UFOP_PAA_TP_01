#ifndef LISTA_ENCADEADA_H_INCLUDED
#define LISTA_ENCADEADA_H_INCLUDED

typedef int tipo_chave;

typedef struct {
    tipo_chave chave;
    /* demais componentes */
} tipo_elemento;

typedef struct nodo {
    tipo_elemento item;
    struct nodo *next;
} tipo_nodo;

typedef struct {
    tipo_nodo *first, *last;
    int tamanho;
} tipo_lista;

typedef tipo_lista* lista;

lista cria_lista();

void termina_lista(lista);

int tamanho_lista(lista);

int insere_apos(lista, tipo_elemento);

int insere_inicio(lista, tipo_elemento);

tipo_chave get_next_chave(lista, tipo_nodo**);

#endif // LISTAENCADEADA_H_INCLUDED
