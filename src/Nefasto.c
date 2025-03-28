#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

bool inserirNefasto(Nefasto **head, Antena *antena, Antena *antena2, int x1, int y1, int x2, int y2)
{
    Nefasto *novo = criarEfeitoNefasto(antena, antena2, x1, y1, x2, y2);
    if (!novo)
        return false;
    novo->next = *head;
    *head = novo;
    return true;
}

Nefasto *criarEfeitoNefasto(Antena *a1, Antena *a2, int x1, int y1, int x2, int y2)
{
    if (!a1 || !a2)
        return NULL;

    Nefasto *novoEfeito = (Nefasto *)malloc(sizeof(Nefasto));
    if (!novoEfeito)
        return NULL;
    novoEfeito->x1 = x1;
    novoEfeito->y1 = y1;
    novoEfeito->x2 = x2;
    novoEfeito->y2 = y2;
    novoEfeito->antena1 = a1;
    novoEfeito->antena2 = a2;
    novoEfeito->next = NULL;
    return novoEfeito;
}

Nefasto *calcularEfeitoNefasto(Antena *lista, int linha, int coluna)
{
    Nefasto *listaNefastos = NULL;

    // Coletar todas as frequências presentes na lista de antenas
    char frequencias[256] = {0};
    Antena *a1 = lista;
    while (a1)
    {
        frequencias[(unsigned char)a1->frequencia] = 1;
        a1 = a1->next;
    }

    // Calcular efeitos nefastos para cada frequência
    for (int i = 0; i < 256; i++)
    {
        if (frequencias[i])
        {
            a1 = lista;
            while (a1)
            {
                if (a1->frequencia == i)
                {
                    Antena *a2 = a1->next;
                    while (a2)
                    {
                        if (a1->frequencia == a2->frequencia)
                        {
                            int tipoEfeito = 0;
                            int dx = a2->x - a1->x;
                            int dy = a2->y - a1->y;
                            int distancia = abs(dx);
                            if (abs(a1->y - a2->y) > distancia)
                                distancia = abs(a1->y - a2->y);
                            if (distancia <= 3)
                            {
                                if (a1->x == a2->x)
                                {
                                    tipoEfeito = 1;
                                }
                                else if (a1->y == a2->y)
                                {
                                    tipoEfeito = 2;
                                }
                                else if (abs(dx) + abs(dy) <= 4)
                                {
                                    tipoEfeito = 3;
                                }
                            }

                            switch (tipoEfeito)
                            {
                            case 1:
                                if (a1->y < a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x, a1->y - distancia, a2->x, a2->y + distancia);
                                }
                                else
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a2->x, a2->y - distancia, a1->x, a1->y + distancia);
                                }
                                break;
                            case 2:
                                if (a1->x < a2->x)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x - distancia, a1->y, a2->x + distancia, a2->y);
                                }
                                else
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a2->x - distancia, a2->y, a1->x + distancia, a1->y);
                                }
                                break;
                            case 3:
                                // Cima esquerda → Baixo direita
                                if (a1->x < a2->x && a1->y < a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x - abs(dx), a1->y - abs(dy), a2->x + abs(dx), a2->y + abs(dy));
                                }
                                // Baixo esquerda → Cima direita
                                else if (a1->x < a2->x && a1->y > a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x - abs(dx), a1->y + abs(dy), a2->x + abs(dx), a2->y - abs(dy));
                                }
                                // Cima direita → Baixo esquerda
                                else if (a1->x > a2->x && a1->y < a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x + abs(dx), a1->y - abs(dy), a2->x - abs(dx), a2->y + abs(dy));
                                }
                                // Baixo direita → Cima esquerda
                                else if (a1->x > a2->x && a1->y > a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x + abs(dx), a1->y + abs(dy), a2->x - abs(dx), a2->y - abs(dy));
                                }

                                /* printf("⚠️ Efeito nefasto entre '%c' em (%d,%d) e (%d,%d)\n",
                                       a1->frequencia, a1->x, a1->y, a2->x, a2->y); */

                                break;
                            default:
                                break;
                            }
                        }
                        a2 = a2->next;
                    }
                }
                a1 = a1->next;
            }
        }
    }

    return listaNefastos;
}

void listarEfeitosNefastos(Nefasto *listaNefastos) {
    if (!listaNefastos) {
        printf("Não existem efeitos nefastos registrados.\n");
        return;
    }

    printf("Lista de efeitos nefastos:\n");
    Nefasto *atual = listaNefastos;
    while (atual) {
        printf("Efeito Nefasto em (%d, %d) e (%d, %d), das antenas '%c' em (%d, %d) e em (%d, %d)\n", 
               atual->x1, atual->y1, atual->x2, atual->y2,atual->antena1->frequencia ,atual->antena1->x, atual->antena1->y, atual->antena2->x, atual->antena2->y);
        atual = atual->next;
    }
}
