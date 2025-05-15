/**
 * @file main.c
 * @author Leonel Freitas (a31520@alunos.ipca.pt)
 * @brief 
 * @version 1.1
 * @date 29-03-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "Funções.h"

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
    do
    {
        mostrarMenuPrincipal();
        opcao = obterOpcaoMenu();
        
        processarOpcao(opcao, lista, efeitos, &linha, &coluna);
    } while (opcao != 3);

    return 0;
}