#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

ListaMensagem *adicionarIdioma(ListaMensagem *lista, int idioma);
ListaMensagem *obterIdiomaPorIndice(ListaMensagem *lista, int idioma);

ListaMensagem *adicionarIdioma(ListaMensagem *lista, int idioma)
{
    // Verifica se o idioma já existe na lista
    ListaMensagem *atual = lista;
    while (atual)
    {
        if (atual->idioma == idioma)
        {
            return lista; // Idioma já existe, não adiciona novamente
        }
        atual = atual->next;
    }

    // Adiciona um novo idioma à lista
    ListaMensagem *novoIdioma = (ListaMensagem *)malloc(sizeof(ListaMensagem));
    if (!novoIdioma)
    {
        printf("Erro ao alocar memória para o idioma.\n");
        return lista;
    }

    novoIdioma->idioma = idioma;
    novoIdioma->mensagens = NULL; // Inicializa a lista de mensagens como vazia
    novoIdioma->next = lista;

    return novoIdioma; // Retorna a nova lista com o idioma adicionado
}

void mostrarMensagem(ListaMensagem *lista, int idioma, int codigo)
{
    ListaMensagem *idiomaLista = obterIdiomaPorIndice(lista, idioma);
    if (!idiomaLista)
    {
        printf("Idioma não encontrado.\n");
        return;
    }

    Mensagem *mensagemAtual = idiomaLista->mensagens;
    while (mensagemAtual)
    {
        if (mensagemAtual->codigo == codigo)
        {
            printf("%s\n", mensagemAtual->texto);
            return;
        }
        mensagemAtual = mensagemAtual->next;
    }

    printf("Mensagem não encontrada.\n");
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

void adicionarMensagem(ListaMensagem *idioma, int codigo, const char *texto)
{
    Mensagem *novaMensagem = (Mensagem *)malloc(sizeof(Mensagem));
    if (!novaMensagem)
    {
        printf("Erro ao alocar memória para a mensagem.\n");
        return;
    }

    novaMensagem->codigo = codigo;
    snprintf(novaMensagem->texto, sizeof(novaMensagem->texto), "%s", texto);
    novaMensagem->next = idioma->mensagens;
    idioma->mensagens = novaMensagem;
}

Mensagem *obterMensagem(ListaMensagem *lista, const char *idioma, int codigo)
{
    // Procura a lista de mensagens para o idioma
    while (lista && lista->idioma != atoi(idioma))
    { // Fix: Convert idioma to int
        lista = lista->next;
    }

    if (!lista)
    {
        return NULL; // Idioma não encontrado
    }

    // Procura a mensagem na lista de mensagens
    Mensagem *mensagemAtual = lista->mensagens;
    while (mensagemAtual)
    {
        if (mensagemAtual->codigo == codigo)
        {
            return mensagemAtual;
        }
        mensagemAtual = mensagemAtual->next;
    }

    return NULL; // Mensagem não encontrada
}

void liberarListaMensagem(ListaMensagem *lista)
{
    while (lista)
    {
        Mensagem *mensagemAtual = lista->mensagens;

        // Libera todas as mensagens do idioma
        while (mensagemAtual)
        {
            Mensagem *tempMensagem = mensagemAtual;
            mensagemAtual = mensagemAtual->next;
            free(tempMensagem);
        }

        // Libera o idioma
        ListaMensagem *tempLista = lista;
        lista = lista->next;
        free(tempLista);
    }
}

ListaMensagem *inicializarListaMensagem()
{
    return NULL; // Inicializa a lista como vazia
}

ListaMensagem *carregarMensagens(const char *caminho)
{
    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o ficheiro de mensagens.\n");
        return NULL;
    }

    ListaMensagem *lista = NULL;
    int proximoIdioma = 1;
    char linha[100];

    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\r\n")] = '\0';

        char *idiomaStrRaw = strtok(linha, ";");
        if (!idiomaStrRaw || strlen(idiomaStrRaw) > 7)
            continue;

        char idiomaStr[8];
        strncpy(idiomaStr, idiomaStrRaw, sizeof(idiomaStr) - 1);
        idiomaStr[sizeof(idiomaStr) - 1] = '\0';
        char *codigoStr = strtok(NULL, ";");
        char *mensagem = strtok(NULL, "");

        if (!*idiomaStr || !*codigoStr || !*mensagem)
        {
            printf("Linha inválida ignorada: %s\n", linha);
            continue;
        }

        // Verificar se idioma já existe
        ListaMensagem *atual = lista;
        while (atual)
        {
            if (strcmp(atual->idiomaStr, idiomaStr) == 0)
                break;
            atual = atual->next;
        }

        // Criar novo idioma se não existir
        if (!atual)
        {
            ListaMensagem *novo = malloc(sizeof(ListaMensagem));
            if (!novo)
            {
                printf("Erro ao alocar memória para idioma.\n");
                fclose(arquivo);
                liberarListaMensagem(lista);
                return NULL;
            }

            memset(novo, 0, sizeof(ListaMensagem));
            strncpy(novo->idiomaStr, idiomaStr, sizeof(novo->idiomaStr));
            novo->idiomaStr[sizeof(novo->idiomaStr) - 1] = '\0';
            novo->idioma = proximoIdioma++;
            novo->mensagens = NULL;
            if (!lista)
            {
                lista = novo;
            }
            else
            {
                ListaMensagem *fim = lista;
                while (fim->next)

                    fim = fim->next;

                fim->next = novo;
            }
            atual = novo;
        }

        // Adicionar nova mensagem a esse idioma
        int codigo = atoi(codigoStr);
        Mensagem *nova = malloc(sizeof(Mensagem));
        if (!nova)
        {
            printf("Erro ao alocar memória para mensagem.\n");
            fclose(arquivo);
            liberarListaMensagem(lista);
            return NULL;
        }

        nova->codigo = codigo;
        strncpy(nova->texto, mensagem, sizeof(nova->texto) - 1);
        nova->texto[sizeof(nova->texto) - 1] = '\0';
        nova->next = atual->mensagens;
        atual->mensagens = nova;
    }

    fclose(arquivo);
    printf("Return da função carregarMensagens :%p\n", (void *)lista);
    
    return lista;
}