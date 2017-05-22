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
TpArvore *insere(TpArvore *arvore);

int main(void) {
    TpArvore *arvore = (TpArvore *)malloc(sizeof(TpArvore));
    arvore = inicializa();
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


void incrementaNivel(TpNodo *nodo) {
    // incrementa o nivel do nodo e seus descendentes
    if (nodo == NULL)
        return;
    nodo->nivel++;
    incrementaNivel(nodo->dir);
    incrementaNivel(nodo->esq);
}

void decrementaNivel(TpNodo *nodo) {
    // decrementa o nivel do nodo e seus descendentes
    if (nodo == NULL)
        return;
    nodo->nivel--;
    decrementaNivel(nodo->dir);
    decrementaNivel(nodo->esq);
}


TpNodo *leftLeft(TpNodo *nodo) {
    TpNodo *a, *b;

    a = nodo;
    b = nodo->esq;

    b->pai = a->pai;
    a->esq = b->dir;
    if (a->esq != NULL)
        a->esq->pai = a;
    a->pai = b;
    b->dir = a;

    b->nivel--;
    a->nivel++;

    decrementaNivel(b->esq);
    incrementaNivel(a->dir);

    nodo = b;

    return nodo;
}

TpNodo *rightRight(TpNodo * nodo) {
    TpNodo *a, *b;

    a = nodo;
    b = nodo->dir;

    b->pai = a->pai;
    a->esq = b->dir;
    if (a->esq != NULL)
        a->esq->pai = a;
    a->pai = b;
    b->dir = a;

    b->nivel--;
    a->nivel++;

    decrementaNivel(b->esq);
    incrementaNivel(a->dir);

    nodo = b;

    return nodo;
}

TpNodo *tio(TpNodo *nodo) { //retorna o tio do nodo
    //se não tem pai ou avô não tem tio
    if (nodo->pai == NULL || nodo->pai->pai == NULL)
        return NULL;

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

TpNodo *consertarRB(TpNodo *nodo, TpNodo *sentinela) {

    TpNodo *tio;

    if (nodo->pai == NULL) { //se o nodo é a raiz
        nodo->cor = BLACK;
        return nodo;
    }

    if (nodo->pai->cor == RED) { //se o pai é vermelho
        if (tio(nodo)->cor == RED) { //se o tio é vermelho
            tio(nodo)->cor = nodo->pai->cor = BLACK;
            nodo->pai->pai->cor = RED;
            return consertarRB(nodo->pai->pai);
        } else if(nodo->chave < nodo->pai->pai->chave){ //se a inseriu na esquerda do avô
            if(nodo->chave > nodo->pai->chave){ //caso 2 e caso 3
                rightRight(nodo->pai);
                troca_cor(nodo);
                troca_cor(nodo->pai);
                leftLeft(nodo->pai);
                return nodo;
            } else{ //caso 3
                troca_cor(nodo->pai);
                troca_cor(nodo->pai->pai);
                leftLeft(nodo->pai->pai);
                return nodo->pai;
            }
        } else{ //se inseriu na direita do avô
            if(nodo->chave > nodo->pai->chave){ //caso 2 e caso 3
                leftLeft(nodo->pai);
                troca_cor(nodo);
                troca_cor(nodo->pai);
                rightRight(nodo->pai);
                return nodo;
            } else{ //caso 3
                troca_cor(nodo->pai);
                troca_cor(nodo->pai->pai);
                rightRight(nodo->pai->pai);
                return nodo->pai;
            }
        }
    }

    return nodo;
}

TpNodo *_insere(TpNodo *pai, TpNodo *nodo, TpNodo *sentinela) {
    nodo->pai = pai;

    if (pai == NULL) {
        nodo->cor = BLACK;
        return nodo;
    }

    if (nodo->chave == pai->chave) {
        free(nodo);
        return pai;
    }

    if (pai->dir == sentinela && nodo->chave > pai->chave) {
        pai->dir = nodo;
    }
    else if (pai->esq == sentinela && nodo->chave < pai->chave) {
        pai->esq = nodo;
    }
    else if (nodo->chave > pai->chave) {
        pai = _insere(pai->dir, nodo, sentinela);
    }
    else {
        pai = _insere(pai->esq, nodo, sentinela);
    }

    pai = consertarRB(pai, sentinela);

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
    return arvore;
}

void _imprime(TpNodo * nodo) {
    if(nodo == NULL) return;
    _imprime(nodo->dir, aux + 1);
    for(int i = 0; i < nodo->nivel; i++) printf("  ");
    printf("%d\n", nodo->val);
    _imprime(nodo->esq, aux + 1);
}
