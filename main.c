#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int dia;
    int mes;
    int ano
}NASCIMENTO;

typedef struct {
    int dia;
    int mes;
    int ano
}ULTIMA_CONSULTA;

typedef struct {
    char *nome;
    char sexo;
    NASCIMENTO nascimento;
    ULTIMA_CONSULTA ultimaConsulta;
}OBJETO_AVL;

typedef struct objeto {
    char *nome;
    char sexo;
    NASCIMENTO nascimento;
    ULTIMA_CONSULTA ultimaConsulta;
    struct objeto *ant;
    struct objeto *prox;
}OBJETO_LISTA;

typedef struct node {
    OBJETO_AVL info;
    int altura;
    struct node *esq;
    struct node *dir;
}NODE;

typedef struct {
    OBJETO_LISTA *inicio;
    int tamanho;
}LISTA_DUPLAMENTE;

// Operações da AVL
void inicializaAVL(NODE **raiz) {
    (*raiz) = NULL;
}

bool estahVazia(NODE **raiz) {
    return (*raiz) == NULL;
}

int maxAlturaFilhos(NODE **no) {
    int hE;
    int hD;
    if((*no)->esq != NULL) hE = (*no)->esq->altura;
    else hE = -1;

    if((*no)->dir != NULL) hD = (*no)->dir->altura;
    else hD = -1;

    return hE > hD ? hE : hD;
}

int calculaFB(NODE **no) {
    
}

// Operações de Lista
void inicializaLista(LISTA_DUPLAMENTE *lista) {
    lista->inicio = NULL;
    int tamanho = 0;
}

bool estahVazia(LISTA_DUPLAMENTE *lista){
    return lista->tamanho == 0;
}

int tamanhoLista(LISTA_DUPLAMENTE *lista){
    return lista->tamanho;
}


int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Erro. Uso: %s <arquivo.asm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    load_program();

    return EXIT_SUCCESS;
}