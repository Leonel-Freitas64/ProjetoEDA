/**
 * @file main.c
 * @author Leonel Freitas (a31520@alunos.ipca.pt)
 * @brief Ficheiro principal do programa.
 * @version 1.1
 * @date 29-03-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

/**
 * @brief Função principal do programa.
 * 
 * @return int 
 */
int main()
{
    Antena* lista = NULL;
    Nefasto* efeitos = NULL;
    int linha = 0, coluna = 0;
    int opcao;
    ListaMensagem* listaMensagens = carregarMensagens("mensagens.txt");
    int idioma = selecionarIdioma(listaMensagens);
    do
    {
        opcao = mostrarMenuPrincipal(listaMensagens, idioma);   
        processarOpcao(opcao, lista, efeitos, &linha, &coluna, listaMensagens, idioma);
    } while (opcao != 3);

    return 0;
}