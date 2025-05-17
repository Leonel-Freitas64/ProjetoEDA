/**
 * @file Nefasto.c
 * @brief Implementação das funções relacionadas aos efeitos nefastos entre antenas.
 * @version 1.1
 * @date 29-03-2025
 */

#include "../resources/Estruturas.h"
#include "../resources/Funções.h"


/**
 * @brief Cria um novo efeito nefasto.
 * 
 * @param a1 Ponteiro para a primeira antena.
 * @param a2 Ponteiro para a segunda antena.
 * @param x1 Coordenada X inicial do efeito.
 * @param y1 Coordenada Y inicial do efeito.
 * @param x2 Coordenada X final do efeito.
 * @param y2 Coordenada Y final do efeito.
 * @return Nefasto* Ponteiro para o novo efeito nefasto ou NULL em caso de erro.
 */
Nefasto* criarEfeitoNefasto(Antena* a1, Antena* a2, int x1, int y1, int x2, int y2)
{
    // Aloca memória para o novo efeito nefasto
    Nefasto* novo = (Nefasto*)malloc(sizeof(Nefasto));
    if (!novo)
        return NULL;

    // Inicializa os campos do efeito nefasto
    novo->antena1 = a1;
    novo->antena2 = a2;
    novo->x1 = x1;
    novo->y1 = y1;
    novo->x2 = x2;
    novo->y2 = y2;
    novo->next = NULL;

    return novo;
}

Nefasto* removerEfeitosPorAntena(Nefasto* listaNefastos, Antena* antena) {
    Nefasto* atual = listaNefastos;
    Nefasto* anterior = NULL;

    while (atual) {
        if (atual->antena1 == antena || atual->antena2 == antena) {
            // Efeito nefasto encontrado, remove da lista
            if (anterior) {
                anterior->next = atual->next;
            } else {
                listaNefastos = atual->next; // Atualiza o início da lista
            }

            // Libera a memória do efeito nefasto
            free(atual);
            atual = (anterior) ? anterior->next : listaNefastos;
        } else {
            anterior = atual;
            atual = atual->next;
        }
    }

    return listaNefastos;
}

/**
 * @brief Libera a memória da lista de efeitos nefastos.
 * 
 * @param listaNefastos Ponteiro para a lista de efeitos nefastos.
 * @return Nefasto* Retorna NULL após liberar a memória.
 */
Nefasto* liberarEfeitosNefastos(Nefasto* listaNefastos)
{
    Nefasto* atual = listaNefastos;
    while (atual)
    {
        Nefasto* temp = atual;
        atual = atual->next;
        free(temp); // Libera a memória do efeito atual
    }
    return NULL; // Retorna NULL para que o chamador possa redefinir o ponteiro
}

/**
 * @brief Adiciona um novo efeito nefasto à lista.
 * 
 * @param listaNefastos Ponteiro para a lista de efeitos nefastos.
 * @param antena1 Primeira antena envolvida no efeito.
 * @param antena2 Segunda antena envolvida no efeito.
 * @param x1 Coordenada X da primeira antena.
 * @param y1 Coordenada Y da primeira antena.
 * @param x2 Coordenada X da segunda antena.
 * @param y2 Coordenada Y da segunda antena.
 * @return Nefasto* Retorna o ponteiro para a lista atualizada.
 */
Nefasto* adicionarEfeitoNefasto(Nefasto* listaNefastos, Antena* antena1, Antena* antena2, int x1, int y1, int x2, int y2)
{
    Nefasto* novo = criarEfeitoNefasto(antena1, antena2, x1, y1, x2, y2);
    if (!novo)
    {
        return listaNefastos; // Retorna a lista sem alterações
    }

    // Insere o novo efeito no início da lista
    novo->next = listaNefastos;
    return novo; // Retorna a nova lista com o efeito adicionado
}

/**
 * @brief Calcula os efeitos nefastos com base na lista de antenas.
 * 
 * @param lista Ponteiro para a lista de antenas.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @return Nefasto* Ponteiro para a lista de efeitos nefastos calculados.
 */
Nefasto* calcularEfeitoNefasto(Antena* lista, int linha, int coluna) {
    Nefasto* listaNefastos = NULL;
    for (Antena* a1 = lista; a1 != NULL; a1 = a1->next) {
        for (Antena* a2 = a1->next; a2 != NULL; a2 = a2->next) {
            if (a1->frequencia != a2->frequencia) continue;

            int dx = abs(a1->x - a2->x);
            int dy = abs(a1->y - a2->y);

            if (dx > 3 || dy > 3) continue;

            Antena* origem = a1;
            Antena* destino = a2;

            // Normalizar a ordem
            if ((a2->x < a1->x) || (a2->x == a1->x && a2->y < a1->y)) {
                origem = a2;
                destino = a1;
            }

            if (origem->x == destino->x) {
                // Vertical
                int yInicio = origem->y - dy;
                int yFim = destino->y + dy;

                listaNefastos = adicionarEfeitoNefasto(listaNefastos, origem, destino, origem->x, yInicio, origem->x, yFim);
            } else if (origem->y == destino->y) {
                // Horizontal
                int xInicio = origem->x - dx;
                int xFim = destino->x + dx;

                listaNefastos = adicionarEfeitoNefasto(listaNefastos, origem, destino, xInicio, origem->y, xFim, origem->y);
            } else if (dx + dy <= 4) {
                // Diagonal
                    int dirX = (destino->x > origem->x) ? dx : -dx;
                    int dirY = (destino->y > origem->y) ? -dy : dy;
                    
                    int xInicio = origem->x - dirX;
                    int yInicio = origem->y + dirY;
                    int xFim = destino->x + dirX;
                    int yFim = destino->y - dirY;

                    listaNefastos = adicionarEfeitoNefasto(listaNefastos, origem, destino, xInicio, yInicio, xFim, yFim);
            
            }
        }
    }
    return listaNefastos;
}

/**
 * @brief Verifica e recalcula os efeitos nefastos com base na lista de antenas.
 * 
 * @param listaAntenas Ponteiro para a lista de antenas.
 * @param listaNefastos Ponteiro para o ponteiro da lista de efeitos nefastos.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @return Nefasto* Ponteiro para a lista de efeitos nefastos recalculados.
 */
Nefasto* verificaEfeitoNefasto(Antena* listaAntenas, Nefasto* listaNefastos, int linha, int coluna)
{
    // Free the existing list of "Nefasto" effects
    if (listaNefastos)
    {
        liberarEfeitosNefastos(listaNefastos);
    }

    // Recalculate the "Nefasto" effects based on the list of antennas
    listaNefastos = calcularEfeitoNefasto(listaAntenas, linha, coluna);

    return listaNefastos;
}

/**
 * @brief Lista todos os efeitos nefastos registrados.
 * 
 * @param listaNefastos Ponteiro para a lista de efeitos nefastos.
 */
void listarEfeitosNefastos(Nefasto* listaNefastos, ListaMensagem* listaMensagens, int idioma)
{
    if (!listaNefastos)
    {
        mostrarMensagem(listaMensagens, idioma, 1100); // Código 1100: "Não existem efeitos nefastos registrados."
        return;
    }

    Nefasto* atual = listaNefastos;
    while (atual)
    {
        Mensagem* mensagem = obterMensagem(listaMensagens, idioma, 1101); // Código 1101: "Efeito Nefasto entre '%c' em (%d, %d) e (%d, %d)"
        if (mensagem)
        {
            printf(mensagem->texto, atual->antena1->frequencia, atual->x1, atual->y1, atual->x2, atual->y2);
        }
        atual = atual->next;
    }
}