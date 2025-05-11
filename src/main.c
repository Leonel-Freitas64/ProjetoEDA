/**
 * @file main.c
 * @author ...
 * @brief 
 * @version 1.1
 * @date 29-03-2025
 * 
 * @copyright ...
 */

#include "../resources/Estruturas.h"
#include <stdint.h>


/**
 * @brief Função para selecionar o idioma.
 * 
 * @return int Retorna o idioma selecionado.
 */
int selecionarIdioma(ListaMensagem* lista) {
    ListaMensagem* atual = lista;
    int opcao = 0;

    printf("Escolha um idioma:\n");
    while (atual) {
        printf("%d. %s\n", atual->idioma, atual->idiomaStr);
        atual = atual->next;
    }

    printf("Digite o número correspondente ao idioma: ");
    scanf("%d", &opcao);

    // Verifica se a opção é válida
    atual = lista;
    while (atual) {
        if (atual->idioma == opcao) {
            return opcao;
        }
        atual = atual->next;
    }

    printf("Idioma inválido! Por favor, escolha novamente.\n");
    return selecionarIdioma(lista); // Chama recursivamente até uma opção válida ser escolhida
}

/**
 * @brief Função principal do programa.
 * 
 * @return int 
 */
int main() {
    Antena* lista = NULL;
    Nefasto* efeitos = NULL;
    int linha = 0, coluna = 0;
    bool primeiraVez = true;
    ListaMensagem* tabelaMensagens = NULL; // Variável global para armazenar a tabela de mensagens

    // Carregar mensagens
    tabelaMensagens = carregarMensagens("src/mensagens.txt");
    printf("Tabeka de mensagens %p\n", (void *)tabelaMensagens);
    if(tabelaMensagens && tabelaMensagens->mensagens) {
        printf("Mensagem 1: %s\n", tabelaMensagens->mensagens->texto);
    } else {
        printf("Erro ao carregar mensagens.\n");
        return 1;
    }
    // Selecionar idioma
    int idioma = selecionarIdioma(tabelaMensagens);

    // Exibir mensagens de teste
    mostrarMensagem(tabelaMensagens, idioma, 1);
    mostrarMensagem(tabelaMensagens, idioma, 404);
    mostrarMensagem(tabelaMensagens, idioma, 100);
    mostrarMensagem(tabelaMensagens, idioma, 101);
    mostrarMensagem(tabelaMensagens, idioma, 102);
    mostrarMensagem(tabelaMensagens, idioma, 103);
    mostrarMensagem(tabelaMensagens, idioma, 104);

    // Liberar memória
    liberarListaMensagem(tabelaMensagens);

    return 1;
}