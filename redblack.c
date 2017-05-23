#include <stdio.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1

typedef struct _nodo {
    int chave;
    int nivel;
    int cor;
    struct _nodo *esq;
    struct _nodo *dir;
    struct _nodo *pai;
} TpNodo;

typedef struct _arvore {
    TpNodo *raiz;
    TpNodo *sentinela;
} TpArvore;

TpArvore *inicializa(void);
TpArvore *insere(TpArvore *arvore, int chave);
void imprime(TpArvore *arvore);
void _imprime(TpNodo * nodo, TpNodo *sentinela);

int treeSize(TpNodo * nodo, TpNodo * sentinela){
    if(nodo == NULL || nodo == sentinela) return 0;
    return 1 + treeSize(nodo->esq, sentinela) + treeSize(nodo->dir, sentinela);
}

int main(void) {
    TpArvore *arvore = (TpArvore *)malloc(sizeof(TpArvore));
    arvore = inicializa();

    int n;

    while(scanf("%d", &n) != EOF){
        arvore = insere(arvore, n);
        printf("\n");
        imprime(arvore);
        printf("size = %d\n", treeSize(arvore->raiz, arvore->sentinela));
        printf("\n");
    }

    printf("fim\n");

    imprime(arvore);
}

TpArvore *inicializa(void) { // aloca memoria para inicializar a arvore
    TpArvore *arvore = (TpArvore *)malloc(sizeof(TpArvore));
    TpNodo *sentinela = (TpNodo *)malloc(sizeof(TpNodo));
    arvore->raiz = NULL;
    arvore->sentinela = sentinela;
    arvore->sentinela->chave = -1;
    arvore->sentinela->cor = BLACK;
    arvore->sentinela->pai = NULL;
    arvore->sentinela->esq = NULL;
    arvore->sentinela->dir = NULL;
    arvore->sentinela->nivel = -1;
    return arvore;
}


void incrementaNivel(TpNodo *nodo, TpNodo * sentinela) {
    // incrementa o nivel do nodo e seus descendentes
    if (nodo == NULL || nodo == sentinela)
        return;
    nodo->nivel++;

    incrementaNivel(nodo->dir, sentinela);
    incrementaNivel(nodo->esq, sentinela);
}

void decrementaNivel(TpNodo *nodo, TpNodo * sentinela) {
    // decrementa o nivel do nodo e seus descendentes
    if (nodo == NULL || nodo == sentinela)
        return;
    nodo->nivel--;
    decrementaNivel(nodo->dir, sentinela);
    decrementaNivel(nodo->esq, sentinela);
}


void leftLeft(TpNodo *nodo, TpNodo * sentinela) {
    TpNodo *a, *b;

    a = nodo;
    b = nodo->esq;

    b->pai = a->pai;
    a->esq = b->dir;
    if (a->esq != NULL){
        a->esq->pai = a;
    }
    if(a->pai != NULL && a->pai->esq == a){
        b->pai->esq = b;
    } else if(a->pai != NULL){
        b->pai->dir = b;
    }
    a->pai = b;
    b->dir = a;

    b->nivel--;
    a->nivel++;

    decrementaNivel(b->esq, sentinela);
    incrementaNivel(a->dir, sentinela);
}

void rightRight(TpNodo * nodo, TpNodo * sentinela) {
    TpNodo *a, *b;

    a = nodo;
    b = nodo->dir;

    b->pai = a->pai;
    a->dir = b->esq;
    if (a->dir != NULL){
        a->dir->pai = a;
    }
    if(a->pai != NULL && a->pai->esq == a){
        b->pai->esq = b;
    } else if(a->pai != NULL){
        b->pai->dir = b;
    }
    a->pai = b;
    b->esq = a;

    b->nivel--;
    a->nivel++;

    decrementaNivel(b->dir, sentinela);
    incrementaNivel(a->esq, sentinela);
}

TpNodo *tio(TpNodo *nodo, TpNodo * sentinela) { //retorna o tio do nodo
    //se não tem pai ou avô não tem tio
    if (nodo->pai == NULL || nodo->pai->pai == NULL)
        return sentinela;

    //se o pai do nodo é o filho da esquerda do avô
    if (nodo->pai == nodo->pai->pai->esq) {
        return nodo->pai->pai->dir;
    }

    //se o pai do nodo é o filho da direita do avô
    return nodo->pai->pai->esq;
}

void troca_cor(TpNodo *nodo){
    if(nodo->cor == BLACK){
        nodo->cor = RED;
    } else{
        nodo->cor = BLACK;
    }
}

TpNodo * encontraRaiz(TpNodo * nodo){
    if(nodo == NULL) {
    }

    if(nodo->pai == NULL){
        return nodo;
    }

    return encontraRaiz(nodo->pai);
}

void consertarRB(TpNodo *nodo, TpNodo *sentinela) {

    TpNodo *tiu = tio(nodo, sentinela);

    if (nodo->pai == NULL) { //se o nodo é a raiz
        nodo->cor = BLACK;
        return;
    }

    if (nodo->pai->cor == RED) { //se o pai é vermelho
        if (tiu->cor == RED) { //se o tio é vermelho
            troca_cor(tiu);
            troca_cor(nodo->pai);
            troca_cor(nodo->pai->pai);
            consertarRB(nodo->pai->pai, sentinela);
                rightRight(nodo->pai, sentinela);
                consertarRB(nodo->esq, sentinela);
            } else{ //caso 3
                troca_cor(nodo->pai);
                troca_cor(nodo->pai->pai);
                _imprime(nodo->pai->pai, sentinela);
                leftLeft(nodo->pai->pai, sentinela);
                _imprime(nodo->pai->pai, sentinela);
            }
        } else{ //se inseriu na direita do avô
            if(nodo == nodo->pai->esq){ //caso 2 e caso 3
                leftLeft(nodo->pai, sentinela);
                consertarRB(nodo->dir, sentinela);
            } else{ //caso 3
                troca_cor(nodo->pai);
                troca_cor(nodo->pai->pai);
                rightRight(nodo->pai->pai, sentinela);
            }
        }
    }
}

TpNodo *_insere(TpNodo *pai, TpNodo *nodo, TpNodo *sentinela) {
    nodo->pai = pai;

    if (pai == NULL) {
        return nodo;
    }

    if (nodo->chave == pai->chave) {
        free(nodo);
        return pai;
    }

    nodo->nivel = pai->nivel + 1;

    if (pai->dir == sentinela && nodo->chave > pai->chave) {
        pai->dir = nodo;
    }
    else if (pai->esq == sentinela && nodo->chave < pai->chave) {
        pai->esq = nodo;
    }
    else if (nodo->chave > pai->chave) {
        pai->dir = _insere(pai->dir, nodo, sentinela);
    }
    else {
        pai->esq = _insere(pai->esq, nodo, sentinela);
    }

    return pai;
}

TpArvore *insere(TpArvore *arvore, int chave){
    TpNodo *nodo = (TpNodo*)malloc(sizeof(TpNodo));
    nodo->cor = RED;
    nodo->esq = nodo->dir = arvore->sentinela;
    nodo->pai = NULL;
    nodo->nivel = 0;
    nodo->chave = chave;
    arvore->raiz = _insere(arvore->raiz, nodo, arvore->sentinela);
    consertarRB(nodo, arvore->sentinela);
    arvore->raiz = encontraRaiz(nodo);
    return arvore;
}

// void _imprime(TpNodo * nodo, TpNodo *sentinela) {
//     if(nodo == NULL || nodo == sentinela) return;
//     _imprime(nodo->dir, sentinela);
//     for(int i = 0; i < nodo->nivel; i++) printf("  ");
//     printf("%d %s\n", nodo->chave, nodo->cor == RED ? "red" : "black");
//     _imprime(nodo->esq, sentinela);
// }

void _imprime(TpNodo *nodo, TpNodo *sentinela){
    if (nodo == NULL || nodo == sentinela) return;
    printf("%d %s\n", nodo->chave, nodo->cor == RED? "red" : "black");
    _imprime(nodo->esq, sentinela);
    _imprime(nodo->dir, sentinela);
}

void imprime(TpArvore *arvore) {
    _imprime(arvore->raiz, arvore->sentinela);
}
