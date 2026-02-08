#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIVROS 50
#define MAX_EMPRESTIMOS 100
#define TAM_STRING 100

struct Livro {
    char nome[TAM_STRING];
    char autor[TAM_STRING];
    char editora[TAM_STRING];
    int edicao;
    int disponivel;
};

struct Emprestimo {
    int indiceLivro;
    char nomeUsuario[TAM_STRING];
};

/* Protótipos */
void limparBufferEntrada();
void exibirMenu();
void cadastrarLivro(struct Livro *biblioteca, int *totalLivros);
void listarLivros(const struct Livro *biblioteca, int totalLivros);
void realizarEmprestimos(struct Livro *biblioteca, int totalLivros,
                         struct Emprestimo *emprestimos, int *totalEmprestimos);
void listarEmprestimos(const struct Livro *biblioteca,
                       const struct Emprestimo *emprestimos, int totalEmprestimos);
void liberarMemoria(struct Livro *biblioteca, struct Emprestimo *emprestimos);

int main() {
    struct Livro *biblioteca = calloc(MAX_LIVROS, sizeof(struct Livro));
    struct Emprestimo *emprestimos = malloc(MAX_EMPRESTIMOS * sizeof(struct Emprestimo));

    if (!biblioteca || !emprestimos) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    int totalLivros = 0;
    int totalEmprestimos = 0;
    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                cadastrarLivro(biblioteca, &totalLivros);
                break;
            case 2:
                listarLivros(biblioteca, totalLivros);
                break;
            case 3:
                realizarEmprestimos(biblioteca, totalLivros, emprestimos, &totalEmprestimos);
                break;
            case 4:
                listarEmprestimos(biblioteca, emprestimos, totalEmprestimos);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    liberarMemoria(biblioteca, emprestimos);
    return 0;
}

/* Funções */

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenu() {
    printf("\n===== BIBLIOTECA =====\n");
    printf("1 - Cadastrar livro\n");
    printf("2 - Listar livros\n");
    printf("3 - Realizar emprestimo\n");
    printf("4 - Listar emprestimos\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
}

void cadastrarLivro(struct Livro *biblioteca, int *totalLivros) {
    if (*totalLivros >= MAX_LIVROS) {
        printf("Biblioteca cheia!\n");
        return;
    }

    printf("Nome: ");
    fgets(biblioteca[*totalLivros].nome, TAM_STRING, stdin);
    biblioteca[*totalLivros].nome[strcspn(biblioteca[*totalLivros].nome, "\n")] = '\0';

    printf("Autor: ");
    fgets(biblioteca[*totalLivros].autor, TAM_STRING, stdin);
    biblioteca[*totalLivros].autor[strcspn(biblioteca[*totalLivros].autor, "\n")] = '\0';

    printf("Editora: ");
    fgets(biblioteca[*totalLivros].editora, TAM_STRING, stdin);
    biblioteca[*totalLivros].editora[strcspn(biblioteca[*totalLivros].editora, "\n")] = '\0';

    printf("Edicao: ");
    scanf("%d", &biblioteca[*totalLivros].edicao);
    limparBufferEntrada();

    biblioteca[*totalLivros].disponivel = 1;
    (*totalLivros)++;

    printf("Livro cadastrado com sucesso!\n");
}

void listarLivros(const struct Livro *biblioteca, int totalLivros) {
    if (totalLivros == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    for (int i = 0; i < totalLivros; i++) {
        printf("\nLivro %d\n", i + 1);
        printf("Nome: %s\n", biblioteca[i].nome);
        printf("Autor: %s\n", biblioteca[i].autor);
        printf("Editora: %s\n", biblioteca[i].editora);
        printf("Edicao: %d\n", biblioteca[i].edicao);
        printf("Status: %s\n", biblioteca[i].disponivel ? "Disponivel" : "Emprestado");
    }
}

void realizarEmprestimos(struct Livro *biblioteca, int totalLivros,
                         struct Emprestimo *emprestimos, int *totalEmprestimos) {

    if (*totalEmprestimos >= MAX_EMPRESTIMOS) {
        printf("Limite de emprestimos atingido.\n");
        return;
    }

    printf("Livros disponiveis:\n");
    for (int i = 0; i < totalLivros; i++) {
        if (biblioteca[i].disponivel) {
            printf("%d - %s\n", i + 1, biblioteca[i].nome);
        }
    }

    int escolha;
    printf("Escolha o livro: ");
    scanf("%d", &escolha);
    limparBufferEntrada();

    int indice = escolha - 1;

    if (indice < 0 || indice >= totalLivros || !biblioteca[indice].disponivel) {
        printf("Livro invalido.\n");
        return;
    }

    printf("Nome do usuario: ");
    fgets(emprestimos[*totalEmprestimos].nomeUsuario, TAM_STRING, stdin);
    emprestimos[*totalEmprestimos].nomeUsuario[strcspn(
        emprestimos[*totalEmprestimos].nomeUsuario, "\n")] = '\0';

    emprestimos[*totalEmprestimos].indiceLivro = indice;
    biblioteca[indice].disponivel = 0;
    (*totalEmprestimos)++;

    printf("Emprestimo realizado com sucesso!\n");
}

void listarEmprestimos(const struct Livro *biblioteca,
                       const struct Emprestimo *emprestimos, int totalEmprestimos) {

    if (totalEmprestimos == 0) {
        printf("Nenhum emprestimo realizado.\n");
        return;
    }

    for (int i = 0; i < totalEmprestimos; i++) {
        int idx = emprestimos[i].indiceLivro;
        printf("\nEmprestimo %d\n", i + 1);
        printf("Livro: %s\n", biblioteca[idx].nome);
        printf("Usuario: %s\n", emprestimos[i].nomeUsuario);
    }
}

void liberarMemoria(struct Livro *biblioteca, struct Emprestimo *emprestimos) {
    free(biblioteca);
    free(emprestimos);
}
