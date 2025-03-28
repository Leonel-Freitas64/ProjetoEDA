#include <stdio.h>
#include <stdlib.h>
#include "Funções.h"


void liberarListaAntenas(Antena *lista) {
    while (lista) {
        Antena *temp = lista;
        lista = lista->next;
        free(temp); // Removed reference to temp->efeitos
    }
}

int main() {
    int opcao, linha, coluna;
    Antena *lista = NULL;
    Nefasto *efeitos = NULL;

    while (1) {
        mostrarMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        processarOpcao(opcao, &lista, &efeitos, &linha, &coluna);
    }

    liberarListaAntenas(lista);
    liberarEfeitosNefastos(efeitos);
    return 0;
}