/**
 * @file Mensagem.c
 * @author Leonel Freitas (a31520@alunos.ipca.pt)
 * @brief Implementação das funções relacionadas à manipulação de mensagens.
 * @version 1.1
 * @date 19-05-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

ListaMensagem *obterIdiomaPorIndice(ListaMensagem *lista, int idioma);

/**
 * @brief Adiciona uma mensagem a um idioma.
 * 
 * @param idioma Ponteiro para o idioma.
 * @param codigo Código da mensagem.
 * @param texto Texto da mensagem.
 * @return int Retorna 1 se a mensagem foi adicionada com sucesso, 0 caso contrário.
 */
int adicionarMensagem(ListaMensagem *idioma, int codigo, const char *texto) {
    if (!idioma) return 0;
    Mensagem *nova = malloc(sizeof(Mensagem));
    if (!nova) return 0;
    nova->codigo = codigo;
    strncpy(nova->texto, texto, MAX_TEXTO - 1);
    nova->texto[MAX_TEXTO - 1] = '\0';
    nova->next = idioma->mensagens;
    idioma->mensagens = nova;
    return 1;
}
/**
 * @brief Mostra uma mensagem específica com base no idioma e código.
 * 
 * @param lista Ponteiro para a lista de idiomas.
 * @param idioma Índice do idioma.
 * @param codigo Código da mensagem.
 * @return bool Retorna true se a mensagem foi exibida com sucesso, false caso contrário.
 */
bool mostrarMensagem(ListaMensagem *lista, int idioma, int codigo) {
    ListaMensagem *idiomaLista = lista;
    while (idiomaLista && idiomaLista->idioma != idioma) {
        idiomaLista = idiomaLista->next;
    }

    if (!idiomaLista) {
        printf("Idioma não encontrado.\n");
        return false;
    }

    Mensagem *mensagemAtual = idiomaLista->mensagens;
    while (mensagemAtual) {
        if (mensagemAtual->codigo == codigo) {
            printf("%s\n", mensagemAtual->texto);
            return true;
        }
        mensagemAtual = mensagemAtual->next;
    }

    printf("Mensagem não encontrada.\n");
    return false;
}

ListaMensagem *obterIdiomaPorIndice(ListaMensagem *lista, int idioma)
{
    while (lista)
    {
        if (lista->idioma == idioma)
        {
            return lista;
        }
        lista = lista->next;
    }
    return NULL; // Idioma não encontrado
}
ListaMensagem *procurarIdioma(ListaMensagem *lista, const char *idiomaStr) {
    while (lista) {
        if (strcmp(lista->idiomaStr, idiomaStr) == 0)
            return lista;
        lista = lista->next;
    }
    return NULL;
}

ListaMensagem *criarIdioma(ListaMensagem *lista, const char *idiomaStr, int idioma) {
    ListaMensagem *novo = malloc(sizeof(ListaMensagem));
    if (!novo) return NULL;
    memset(novo, 0, sizeof(ListaMensagem));
    strncpy(novo->idiomaStr, idiomaStr, MAX_IDIOMA - 1);
    novo->idiomaStr[MAX_IDIOMA - 1] = '\0';
    novo->idioma = idioma;
    novo->mensagens = NULL;
    novo->next = lista;
    return novo;
}


Mensagem *obterMensagem(ListaMensagem *lista, int idioma, int codigo) {
    // Procura a lista de mensagens para o idioma
    while (lista && lista->idioma != idioma) {
        lista = lista->next;
    }

    if (!lista) {
        return NULL; // Idioma não encontrado
    }

    // Procura a mensagem na lista de mensagens
    Mensagem *mensagemAtual = lista->mensagens;
    while (mensagemAtual) {
        if (mensagemAtual->codigo == codigo) {
            return mensagemAtual;
        }
        mensagemAtual = mensagemAtual->next;
    }

    return NULL; // Mensagem não encontrada
}


ListaMensagem* carregarMensagens(const char* caminho) {
    FILE* arquivo = fopen(caminho, "r");
    if (!arquivo) {
        return NULL;
    }

    ListaMensagem* lista = NULL;
    char linha[200];
    int proximoIdioma = 1;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\r\n")] = '\0'; // Remove \n ou \r\n do final

        char* idiomaStr = strtok(linha, ";");
        char* codigoStr = strtok(NULL, ";");
        char* texto = strtok(NULL, "");

        if (!idiomaStr || !codigoStr || !texto || strlen(idiomaStr) == 0 || strlen(codigoStr) == 0 || strlen(texto) == 0) {
            continue;
        }

        // Substituição de "\n" por quebra de linha real
        char textoFinal[MAX_TEXTO];
        int i = 0, j = 0;
        while (texto[i] && j < MAX_TEXTO - 1) {
            if (texto[i] == '\\' && texto[i + 1] == 'n') {
                textoFinal[j++] = '\n';
                i += 2;
            } else {
                textoFinal[j++] = texto[i++];
            }
        }
        textoFinal[j] = '\0';

        // Verificar se o idioma já existe
        ListaMensagem* atual = lista;
        while (atual) {
            if (strcmp(atual->idiomaStr, idiomaStr) == 0) break;
            atual = atual->next;
        }

        // Criar novo idioma se necessário
        if (!atual) {
            ListaMensagem* novo = (ListaMensagem*) malloc(sizeof(ListaMensagem));
            if (!novo) {
                fclose(arquivo);
                return lista;
            }

            memset(novo, 0, sizeof(ListaMensagem));
            strncpy(novo->idiomaStr, idiomaStr, sizeof(novo->idiomaStr) - 1);
            novo->idiomaStr[sizeof(novo->idiomaStr) - 1] = '\0';
            novo->idioma = proximoIdioma++;
            novo->mensagens = NULL;
            novo->next = NULL;
            if (!lista) {
                lista = novo;
            } else {
                ListaMensagem* aux = lista;
                while (aux->next) {
                    aux = aux->next;
                }
                aux->next = novo;
            }
            atual = novo; 
        }

        // Criar nova mensagem
        Mensagem* novaMensagem = (Mensagem*) malloc(sizeof(Mensagem));
        if (!novaMensagem) {
            fclose(arquivo);
            return lista;
        }

        novaMensagem->codigo = atoi(codigoStr);
        strncpy(novaMensagem->texto, textoFinal, sizeof(novaMensagem->texto) - 1);
        novaMensagem->texto[sizeof(novaMensagem->texto) - 1] = '\0';
        novaMensagem->next = NULL;
        if(!atual->mensagens) {
            atual->mensagens = novaMensagem;
        } else {
            Mensagem* aux = atual->mensagens;
            while (aux->next) {
                aux = aux->next;
            }
            aux->next = novaMensagem;
        }
    }

    fclose(arquivo);
    return lista;
}

/**
 * @brief Seleciona o idioma com base na lista de mensagens.
 * 
 * @param lista Ponteiro para a lista de mensagens.
 * @return int Retorna o índice do idioma selecionado.
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
     // Limpa o buffer de entrada
    limparBufferEntrada();

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
 * @brief Mostra uma mensagem de sobreposição detectada em coordenadas específicas.
 *
 * @param listaMensagens Ponteiro para a lista de mensagens.
 * @param idioma Idioma selecionado.
 * @param x Coordenada X onde a sobreposição foi detectada.
 * @param y Coordenada Y onde a sobreposição foi detectada.
 * @param caractere O caractere que já existia na posição.
*/
int mostrarSobreposicao(ListaMensagem *listaMensagens, int idioma, int x, int y, char caractere)
{
    // Obtém a mensagem base para sobreposição (código 600 no mensagens.txt)
    Mensagem *mensagem = obterMensagem(listaMensagens, idioma, 600);
    if (!mensagem)
    {
        return 0;
    }

    // Exibe a mensagem formatada com as coordenadas e o caractere
    printf(mensagem->texto, x, y, caractere);
    return 1;
}


