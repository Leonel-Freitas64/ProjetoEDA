#include "../resources/Funções.h"
#include "../resources/Estruturas.h"
Antena *criarAntena(char freq, int x, int y)
{
    Antena *nova = malloc(sizeof(Antena));
    if (!nova)
        return NULL;
    memset(nova, 0, sizeof(Antena));
    nova->frequencia = freq;
    nova->x = x;
    nova->y = y;
    nova->next = NULL;
    return nova;
}

// Insere no início da lista
bool inserirAntena(Antena **head, char freq, int x, int y) {
    // Verificar se já existe uma antena nas mesmas coordenadas
    Antena *current = *head;
    while (current) {
        if (current->x == x && current->y == y) {
            printf("Erro: Já existe uma antena nas coordenadas (%d, %d)\n", x, y);
            return false;
        }
        current = current->next;
    }

    // Inserir nova antena
    Antena *nova = criarAntena(freq, x, y);
    if (!nova) return false;
    nova->next = *head;
    *head = nova;
    return true;
}

bool eliminarAntena(Antena** head, int x, int y) {
    if (!head || !*head) {
        return false; // Lista vazia
    }

    Antena* atual = *head;
    Antena* anterior = NULL;

    // Percorre a lista para encontrar a antena com as coordenadas (x, y)
    while (atual) {
        if (atual->x == x && atual->y == y) {
            // Caso a antena seja encontrada
            if (anterior) {
                // Se não for a primeira antena, ajusta o ponteiro do anterior
                anterior->next = atual->next;
            } else {
                // Se for a primeira antena, ajusta o head
                *head = atual->next;
            }
            free(atual); // Libera a memória da antena removida
            return true; // Antena removida com sucesso
        }
        anterior = atual;
        atual = atual->next;
    }

    return false; // Antena não encontrada
}

void recolherDadosAntena(char* freq, int* x, int* y, int linha, int coluna) {
    do {
        printf("Digite a frequência da antena (A-Z, a-z, 0-9): ");
        scanf(" %c", freq);
        if (!((*freq >= 'A' && *freq <= 'Z') || (*freq >= 'a' && *freq <= 'z') || (*freq >= '0' && *freq <= '9'))) {
            printf("Frequência inválida! Deve ser uma letra (A-Z, a-z) ou um número (0-9).\n");
        }
    } while (!((*freq >= 'A' && *freq <= 'Z') || (*freq >= 'a' && *freq <= 'z') || (*freq >= '0' && *freq <= '9')));

    do {
        printf("Digite a posição X da antena (0 a %d): ", coluna - 1);
        scanf("%d", x);
        if (*x < 0 || *x >= coluna) {
            printf("Posição X inválida! Deve estar entre 0 e %d.\n", coluna - 1);
        }
    } while (*x < 0 || *x >= coluna);

    do {
        printf("Digite a posição Y da antena (0 a %d): ", linha - 1);
        scanf("%d", y);
        if (*y < 0 || *y >= linha) {
            printf("Posição Y inválida! Deve estar entre 0 e %d.\n", linha - 1);
        }
    } while (*y < 0 || *y >= linha);
}

// Lê ficheiro e guarda apenas antenas
Antena *lerAntenasDoFicheiro(const char *nomeFicheiro, int *linha, int *coluna)
{
    FILE *f = fopen(nomeFicheiro, "r");
    if (!f)
    {
        printf("Erro ao abrir o ficheiro '%s'\n", nomeFicheiro);
        return NULL;
    }

    Antena *lista = NULL;
    char buffer[256];
    int y = 0;
    int maxColuna = 0;

    while (fgets(buffer, sizeof(buffer), f))
    {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        int linhaComprimento = strlen(buffer);
        if (linhaComprimento > maxColuna)
        {
            maxColuna = linhaComprimento;
        }
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            char c = buffer[i];
            if (isalpha(c) || isdigit(c))
            {
                if (inserirAntena(&lista, c, i, y) == true)
                {
                    // Inserção bem-sucedida, prossegue
                }
                else
                {
                    // Inserção falhou, trata o erro
                    printf("Erro ao inserir antena na posição (%d, %d)\n", i, y);
                }
            }
        }
        y++;
    }

    *linha = y;
    *coluna = maxColuna;

    fclose(f);
    return lista;
}

// Imprime antenas
void listarAntenas(Antena *lista)
{
    Antena *current = lista;
    while (current)
    {
        //printf("Frequencia: %c, Coordenadas: (%d, %d)\n", current->frequencia, current->x, current->y);
        current = current->next;
    }
}
