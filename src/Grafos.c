/**
 * @file Grafos.c
 * @author Leonel Freitas (a31520@alunos.ipca.pt)
 * @brief Implementação das funções relacionadas à manipulação de grafos.
 * @version 1.1
 * @date 19-05-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

/**
 * @brief Obtém ou cria um vértice no grafo para uma antena.
 *
 * @param grafo Ponteiro para o grafo.
 * @param antena Ponteiro para a antena.
 * @return Vertice* Ponteiro para o vértice correspondente à antena.
 */
static Vertice *obterOuCriarVertice(Grafo *grafo, Antena *antena)
{
    if (!antena)
    {
        return NULL;
    }

    Vertice *atual = grafo->vertices;
    while (atual)
    {
        if (atual->antena == antena)
            return atual;
        atual = atual->next;
    }

    // Cria um novo vértice se não encontrado
    Vertice *novo = malloc(sizeof(Vertice));
    if (!novo)
    {
        return NULL;
    }

    novo->antena = antena;
    novo->adjacentes = NULL;
    novo->next = grafo->vertices;
    grafo->vertices = novo;

    return novo;
}

/**
 * @brief Encontra um vértice no grafo com base na antena.
 *
 * @param atual Ponteiro para o vértice inicial.
 * @param antena Ponteiro para a antena.
 * @return Vertice* Ponteiro para o vértice encontrado ou NULL se não encontrado.
 */
Vertice *encontrarVerticePorAntena(Vertice *atual, Antena *antena)
{
    while (atual)
    {
        if (atual->antena == antena)
        {
            return atual;
        }
        atual = atual->next;
    }
    return NULL;
}

/**
 * @brief Adiciona uma aresta entre duas antenas no grafo.
 *
 * @param grafo Ponteiro para o grafo.
 * @param a Ponteiro para a antena de origem.
 * @param b Ponteiro para a antena de destino.
 * @return int Retorna 0 se a aresta foi adicionada com sucesso, 1 caso contrário.
 */
int adicionarAresta(Grafo *grafo, Antena *a, Antena *b)
{
    if (!grafo || !a || !b)
        return 1;

    Vertice *va = obterOuCriarVertice(grafo, a);
    Vertice *vb = obterOuCriarVertice(grafo, b);
    if (!va || !vb)
        return 1;

    Aresta *ab = malloc(sizeof(Aresta));
    if (!ab)
        return 1;
    ab->destino = b;
    ab->next = va->adjacentes;
    va->adjacentes = ab;

    Aresta *ba = malloc(sizeof(Aresta));
    if (!ba)
        return 1;
    ba->destino = a;
    ba->next = vb->adjacentes;
    vb->adjacentes = ba;

    return 0;
}

/**
 * @brief Constrói um grafo com base na lista de antenas.
 *
 * @param lista Ponteiro para a lista de antenas.
 * @param listamensagens Ponteiro para a lista de mensagens.
 * @param idioma Idioma selecionado.
 * @return Grafo* Ponteiro para o grafo construído ou NULL em caso de erro.
 */
Grafo *construirGrafo(Antena *lista)
{
    Grafo *grafo = malloc(sizeof(Grafo));
    if (!grafo)
        return NULL;
    grafo->vertices = NULL;

    for (Antena *a = lista; a != NULL; a = a->next)
    {
        for (Antena *b = a->next; b != NULL; b = b->next)
        {
            if (a != b && a->frequencia == b->frequencia)
            {
                int dx = abs(a->x - b->x);
                int dy = abs(a->y - b->y);
                if (dx > 3 || dy > 3)
                    continue;
                if (a->x == b->x && dy <= 3)
                {
                adicionarAresta(grafo, a, b);
                }
                else if (a->y == b->y && dx <= 3)
                {
                    adicionarAresta(grafo, a, b);
                }
                else if ((dx + dy <= 5))
                {
                    adicionarAresta(grafo, a, b);
                }
            }
        }
    }

    return grafo;
}


/**
 * @brief Imprime o grafo com as conexões entre antenas.
 *
 * @param grafo Ponteiro para o grafo.
 * @param lista Ponteiro para a lista de antenas.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @param listaMensagens Ponteiro para a lista de mensagens.
 * @param idioma Idioma selecionado.
 * @return int Retorna 1 se o grafo foi impresso com sucesso, 0 caso contrário.
 */
int imprimirGrafo(Grafo *grafo, Antena *lista, int linha, int coluna, ListaMensagem *listaMensagens, int idioma)
{
    if (!grafo || !lista)
    {
        mostrarMensagem(listaMensagens, idioma, 902); // Código 1000: "Erro: Grafo ou lista de antenas é nulo."
        return 0;
    }

    mostrarMensagem(listaMensagens, idioma, 903); // Código 1001: "Grafo:\n"

    // Itera sobre os vértices do grafo
    Vertice *v = grafo->vertices;
    while (v)
    {
        Mensagem *mensagemVertice = obterMensagem(listaMensagens, idioma, 904); // Código 1002: "Antena de frequencia %c: (%d, %d) --- "
        if (mensagemVertice)
        {
            printf(mensagemVertice->texto, v->antena->frequencia, v->antena->x, v->antena->y);
        }

        // Itera sobre as arestas (adjacências) do vértice
        Aresta *adj = v->adjacentes;
        while (adj)
        {
            Mensagem *mensagemAresta = obterMensagem(listaMensagens, idioma, 905); // Código 1003: "Antena (%d, %d)"
            if (mensagemAresta)
            {
                printf(mensagemAresta->texto, adj->destino->x, adj->destino->y);
            }

            adj = adj->next;
            if (adj)
            {
                printf(" --> ");
            }
        }

        printf("\n"); // Nova linha para o próximo vértice
        v = v->next;
    }
    return 1;
}

int orient(int xa, int ya, int xb, int yb, int xc, int yc)
{
    int valor = (yb - ya) * (xc - xb) - (xb - xa) * (yc - yb);
    if (valor == 0)
        return 0;
    return (valor > 0) ? 1 : 2; // 1 para horário, 2 para anti-horário
}

bool onSegment(int x1, int y1, int x2, int y2, int px, int py)
{
    return (px >= (x1 < x2 ? x1 : x2) && px <= (x1 > x2 ? x1 : x2) &&
            py >= (y1 < y2 ? y1 : y2) && py <= (y1 > y2 ? y1 : y2));
}

bool intersecao(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    int o1 = orient(x1, y1, x2, y2, x3, y3);
    int o2 = orient(x1, y1, x2, y2, x4, y4);
    int o3 = orient(x3, y3, x4, y4, x1, y1);
    int o4 = orient(x3, y3, x4, y4, x2, y2);

    // Verifica se os segmentos se cruzam
    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(x1, y1, x2, y2, x3, y3))
        return true;
    if (o2 == 0 && onSegment(x1, y1, x2, y2, x4, y4))
        return true;
    if (o3 == 0 && onSegment(x3, y3, x4, y4, x1, y1))
        return true;
    if (o4 == 0 && onSegment(x3, y3, x4, y4, x2, y2))
        return true;
    return false;
}

/**
 * @brief Detecta interseções teóricas entre antenas no grafo.
 *
 * @param grafo Ponteiro para o grafo.
 * @param listamensagens Ponteiro para a lista de mensagens.
 * @param idioma Idioma selecionado.
 * @return int Número de interseções detectadas.
 */
int detetarIntersecoes(Grafo *grafo, ListaMensagem *listamensagens, int idioma)
{
    float pontosX[1000], pontosY[1000];
    char freq1[1000], freq2[1000];
    int total = 0;
    Mensagem *mensagem = obterMensagem(listamensagens, idioma, 908); // "Interseções teóricas entre antenas:\n"

    for (Vertice *v1 = grafo->vertices; v1 != NULL; v1 = v1->next)
    {
        for (Aresta *a1 = v1->adjacentes; a1 != NULL; a1 = a1->next)
        {
            Antena *a1_1 = v1->antena;
            Antena *a1_2 = a1->destino;
            if (!a1_1 || !a1_2)
                continue;
            if (a1_1->frequencia != a1_2->frequencia)
                continue;

            for (Vertice *v2 = grafo->vertices; v2 != NULL; v2 = v2->next)
            {
                for (Aresta *a2 = v2->adjacentes; a2 != NULL; a2 = a2->next)
                {
                    Antena *a2_1 = v2->antena;
                    Antena *a2_2 = a2->destino;
                    if (!a2_1 || !a2_2)
                        continue;
                    if (a2_1->frequencia != a2_2->frequencia)
                        continue;
                    if (a1_1->frequencia == a2_1->frequencia)
                        continue;

                    // Evitar pares exatamente iguais
                    if ((a1_1 == a2_1 && a1_2 == a2_2) || (a1_1 == a2_2 && a1_2 == a2_1))
                        continue;
                    // Verificar se já foi impresso
                    int repetido = 0;
                    for (int i = 0; i < total; i++)
                    {
                        if (((freq1[i] == a1_1->frequencia && freq2[i] == a2_1->frequencia) ||
                             (freq2[i] == a1_1->frequencia && freq1[i] == a2_1->frequencia)))
                        {
                            repetido = 1;
                            break;
                        }
                    }

                    if (!repetido)
                    {
                        if (intersecao(a1_1->x, a1_1->y, a1_2->x, a1_2->y,
                                       a2_1->x, a2_1->y, a2_2->x, a2_2->y))
                        {
                            float x = (a1_1->x + a1_2->x + a2_1->x + a2_2->x) / 4.0;
                            float y = (a1_1->y + a1_2->y + a2_1->y + a2_2->y) / 4.0;

                            if (mensagem)
                            {
                                printf(mensagem->texto, a1_1->frequencia, a2_1->frequencia, x, y);
                            }
                            pontosX[total] = x;
                            pontosY[total] = y;
                            freq1[total] = a1_1->frequencia;
                            freq2[total] = a2_1->frequencia;
                            total++;
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Executa a pesquisa em profundidade (DFS) a partir de uma antena.
 *
 * @param grafo Ponteiro para o grafo.
 * @param origem Antena de origem para iniciar a pesquisa.
 * @param listaMensagens Ponteiro para a lista de mensagens.
 * @param idioma Idioma selecionado.
 * @return int Número de antenas visitadas.
 */
ResultadoDFS* dfs(Grafo *grafo, Antena *origem) {
    // Aloca memória para o resultado
    ResultadoDFS *res = malloc(sizeof(ResultadoDFS));
    if (!res || !grafo || !origem) {
        return NULL; // Retorna NULL em caso de erro
    }

    // Inicializa o resultado
    res->total = 0;
    memset(res->x, 0, sizeof(res->x));
    memset(res->y, 0, sizeof(res->y));

    // Inicializa a matriz de visitados e a pilha
    int visitado[10000] = {0}; // Assume matriz até 100x100
    Vertice *pilha[100];       // Pilha para DFS
    int topo = 0;

    // Obtém o vértice inicial
    Vertice *inicio = obterOuCriarVertice(grafo, origem);
    if (!inicio) {
        free(res);
        return NULL; // Retorna NULL se o vértice inicial não puder ser obtido
    }

    // Adiciona o vértice inicial à pilha
    pilha[topo++] = inicio;

    // Executa a pesquisa em profundidade
    while (topo > 0) {
        // Remove o vértice do topo da pilha
        Vertice *atual = pilha[--topo];
        int idx = atual->antena->x * 100 + atual->antena->y;

        // Ignora vértices já visitados
        if (visitado[idx]) continue;

        // Marca o vértice como visitado
        visitado[idx] = 1;

        // Adiciona as coordenadas ao resultado
        if (res->total < 100) { // Evita estouro do array
            res->x[res->total] = atual->antena->x;
            res->y[res->total] = atual->antena->y;
            res->total++;
        } else {
            break;//Excedido os 100
        }

        // Adiciona os vértices adjacentes à pilha
        Aresta *adj = atual->adjacentes;
        while (adj) {
            Vertice *vizinho = obterOuCriarVertice(grafo, adj->destino);
            int id_vizinho = vizinho->antena->x * 100 + vizinho->antena->y;

            if (!visitado[id_vizinho]) {
                pilha[topo++] = vizinho;
            }
            adj = adj->next;
        }
    }

    return res; // Retorna o resultado com as coordenadas das antenas visitadas
}


/**
 * @brief Mostra o resultado da pesquisa em profundidade (DFS).
 *
 * @param res Ponteiro para o resultado contendo as coordenadas das antenas visitadas.
 * @param listaMensagens Ponteiro para a lista de mensagens.
 * @param idioma Idioma selecionado.
 * @return int Retorna 1 se o resultado foi exibido com sucesso, 0 se houve erro.
 */
int mostrarResultadoDFS(ResultadoDFS *res, ListaMensagem *listaMensagens, int idioma) {
    if (!res) {
        mostrarMensagem(listaMensagens, idioma, 902); // Código 902: "Erro: Resultado DFS é nulo."
        return 0; // Retorna 0 em caso de erro
    }

    Mensagem *mensagem = obterMensagem(listaMensagens, idioma, 907); // Código 907: "[DFS] Visitado: '%c' em (%d, %d)"
    if (!mensagem) {
        mostrarMensagem(listaMensagens, idioma, 406); // Código 406: "Erro a ler o ficheiro"
        return 0;
    }

    for (int i = 0; i < res->total; i++) {
        printf(mensagem->texto, res->x[i], res->y[i]);
    }

    return 1; // Retorna 1 indicando sucesso
}
/**
 * @brief Executa a pesquisa em largura (BFS) a partir de uma antena.
 *
 * @param grafo Ponteiro para o grafo.
 * @param origem Antena de origem para iniciar a pesquisa.
 * @param listaMensagens Ponteiro para a lista de mensagens.
 * @param idioma Idioma selecionado.
 * @return int Número de antenas visitadas.
 */
ResultadoBFS* bfs(Grafo *grafo, Antena *origem) {
    // Aloca memória para o resultado
    ResultadoBFS *res = malloc(sizeof(ResultadoBFS));
    if (!res || !grafo || !origem) {
        return NULL; // Retorna NULL em caso de erro
    }

    // Inicializa o resultado
    res->total = 0;
    memset(res->x, 0, sizeof(res->x));
    memset(res->y, 0, sizeof(res->y));

    // Inicializa a matriz de visitados e a fila
    int visitado[10000] = {0}; // Assume matriz até 100x100
    Vertice *fila[100];        // Fila para BFS
    int frente = 0, tras = 0;

    // Obtém o vértice inicial
    Vertice *inicio = obterOuCriarVertice(grafo, origem);
    if (!inicio) {
        free(res);
        return NULL; // Retorna NULL se o vértice inicial não puder ser obtido
    }

    // Adiciona o vértice inicial à fila
    fila[tras++] = inicio;

    // Executa a pesquisa em largura
    while (frente < tras) {
        // Remove o vértice da frente da fila
        Vertice *atual = fila[frente++];
        int idx = atual->antena->x * 100 + atual->antena->y;

        // Ignora vértices já visitados
        if (visitado[idx]) continue;

        // Marca o vértice como visitado
        visitado[idx] = 1;

        // Adiciona as coordenadas ao resultado
        if (res->total < 100) { // Evita estouro do array
            res->x[res->total] = atual->antena->x;
            res->y[res->total] = atual->antena->y;
            res->total++;
        } else {
            break;//Execedido os 100
        }

        // Adiciona os vértices adjacentes à fila
        Aresta *adj = atual->adjacentes;
        while (adj) {
            Vertice *vizinho = obterOuCriarVertice(grafo, adj->destino);
            int id_vizinho = vizinho->antena->x * 100 + vizinho->antena->y;

            if (!visitado[id_vizinho]) {
                fila[tras++] = vizinho;
            }
            adj = adj->next;
        }
    }

    return res; // Retorna o resultado com as coordenadas das antenas visitadas
}

/**
 * @brief Mostra o resultado da pesquisa em largura (BFS).
 *
 * @param res Ponteiro para o resultado contendo as coordenadas das antenas visitadas.
 * @param listaMensagens Ponteiro para a lista de mensagens.
 * @param idioma Idioma selecionado.
 * @return int Retorna 1 se o resultado foi exibido com sucesso, 0 se houve erro.
 */
int mostrarResultadoBFS(ResultadoBFS *res, ListaMensagem *listaMensagens, int idioma) {
    if (!res) {
        mostrarMensagem(listaMensagens, idioma, 902); // Código 902: "Erro: Resultado BFS é nulo."
        return 0; // Retorna 0 em caso de erro
    }

    Mensagem *mensagem = obterMensagem(listaMensagens, idioma, 907); // Código 907: "[BFS] Visitado: '%c' em (%d, %d)"
    if (!mensagem) {
        mostrarMensagem(listaMensagens, idioma, 406); // Código 406: "Erro a ler o ficheiro"
        return 0;
    }

    for (int i = 0; i < res->total; i++) {
        printf(mensagem->texto, res->x[i], res->y[i]);
    }

    return 1; // Retorna 1 indicando sucesso
}
/**
 * @brief Função recursiva para encontrar e exibir todos os caminhos possíveis entre dois vértices no grafo.
 *
 * @param atual Ponteiro para o vértice atual sendo visitado.
 * @param destino Ponteiro para a antena de destino.
 * @param visitado Array para marcar os vértices já visitados.
 * @param caminhoX Array para armazenar as coordenadas X do caminho atual.
 * @param caminhoY Array para armazenar as coordenadas Y do caminho atual.
 * @param profundidade Profundidade atual da recursão, usada para indexar os arrays de caminho.
 * @return int Retorna 1 se o caminho foi encontrado, 0 caso contrário.
 */
int dfsCaminhosRec(Vertice *atual, Antena *destino, int *visitado, int *caminhoX, int *caminhoY, int profundidade)
{
    if (!atual || !destino)
    {
        return 0; // Retorna 0 se o vértice atual ou o destino forem inválidos
    }

    int index = atual->antena->x * 100 + atual->antena->y; // Calcula o índice único para o vértice atual
    visitado[index] = 1;                                   // Marca o vértice como visitado

    // Adiciona a antena atual ao caminho
    caminhoX[profundidade] = atual->antena->x;
    caminhoY[profundidade] = atual->antena->y;

    // Verifica se o destino foi alcançado
    if (atual->antena->x == destino->x && atual->antena->y == destino->y)
    {
        // Exibe o caminho encontrado
        for (int i = 0; i <= profundidade; i++)
        {
            printf("(%d, %d) ", caminhoX[i], caminhoY[i]);
        }
        printf("\n");
        visitado[index] = 0; // Desmarca o vértice antes de retornar
        return 1;
    }

    // Explora os vértices adjacentes
    Aresta *adj = atual->adjacentes;
    while (adj)
    {
        Antena *destinoAntena = adj->destino;
        int destinoIndex = destinoAntena->x * 100 + destinoAntena->y;
        if (!visitado[destinoIndex])
        {
            Vertice *proximo = encontrarVerticePorAntena(atual, destinoAntena);
            dfsCaminhosRec(proximo, destino, visitado, caminhoX, caminhoY, profundidade + 1);
        }
        adj = adj->next;
    }

    visitado[index] = 0; // Desmarca o vértice
    return 0;
}

/**
 * @brief Mostra todos os caminhos possíveis entre duas antenas no grafo, utilizando DFS recursivo.
 *
 * @param grafo Ponteiro para o grafo onde o caminho será procurado.
 * @param origem Antena de origem.
 * @param destino Antena de destino.
 */
bool mostrarCaminhos(Grafo *grafo, Antena *origem, Antena *destino)
{
    if (!grafo || !origem || !destino)
    {
        return false;
    }

    Vertice *verticeOrigem = obterOuCriarVertice(grafo, origem);
    if (!verticeOrigem)
    {
        return false;
    }

    int visitado[10000] = {0};        // Array para marcar os vértices visitados
    int caminhoX[100], caminhoY[100]; // Arrays para armazenar o caminho

    dfsCaminhosRec(verticeOrigem, destino, visitado, caminhoX, caminhoY, 0);
    return true;
}

/**
 * @brief Libera a memória do Grafo.
 *
 * @param lista Ponteiro para o Grafo.
 * @return Grafo* Retorna NULL após liberar a memória.
 */
Grafo *libertarGrafo(Grafo *grafo)
{
    if (!grafo)
        return NULL;

    Vertice *vAtual = grafo->vertices;
    while (vAtual)
    {
        // Libertar todas as arestas do vértice atual
        Aresta *aAtual = vAtual->adjacentes;
        while (aAtual)
        {
            Aresta *aRemover = aAtual;
            aAtual = aAtual->next;
            free(aRemover);
        }

        // Libertar o vértice
        Vertice *vRemover = vAtual;
        vAtual = vAtual->next;
        free(vRemover);
    }

    // Libertar o grafo
    free(grafo);
    return NULL;
}