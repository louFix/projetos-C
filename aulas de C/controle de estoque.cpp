/*
 * PROGRAMA BASICO DE CONTROLE DE FLUXO - CONTROLE DE ESTOQUE
 *
 * Situacao simulada: estoque de uma pequena loja.
 *
 * Estruturas utilizadas:
 *   - if / else if / else ..... validacoes e decisoes
 *   - switch .................. menu de opcoes
 *   - for ..................... cadastro, listagem e relatorio
 *   - while ................... busca de produto no vetor
 *   - do-while ................ menu principal e validacao de entrada
 *   - vetores (arrays) ........ codigos, nomes, quantidades e precos
 *
 * Compilar: gcc controle_estoque.c -o controle_estoque
 * Executar: ./controle_estoque
 */

#include <stdio.h>
#include <string.h>

#define MAX_PRODUTOS    50
#define TAM_NOME        30
#define ESTOQUE_MINIMO  5

/* Busca um produto pelo codigo usando WHILE.
   Retorna a posicao no vetor ou -1 se nao encontrar. */
int buscarProduto(int codigos[], int total, int codigoBuscado) {
    int i = 0;
    while (i < total) {
        if (codigos[i] == codigoBuscado) {
            return i;
        }
        i++;
    }
    return -1;
}

int main(void) {
    /* Vetores paralelos: a posicao i representa o mesmo produto em todos */
    int   codigos[MAX_PRODUTOS];
    char  nomes[MAX_PRODUTOS][TAM_NOME];
    int   quantidades[MAX_PRODUTOS];
    float precos[MAX_PRODUTOS];

    int   totalProdutos = 0;
    float faturamento = 0.0f;
    int   opcao;

    /* DO-WHILE: o menu aparece pelo menos uma vez e repete ate sair */
    do {
        printf("\n===== CONTROLE DE ESTOQUE =====\n");
        printf("1 - Cadastrar produtos\n");
        printf("2 - Listar estoque\n");
        printf("3 - Registrar venda\n");
        printf("4 - Repor estoque\n");
        printf("5 - Relatorio geral\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        /* SWITCH: escolhe a acao conforme a opcao do menu */
        switch (opcao) {

        case 1: {
            int qtd;
            printf("\nQuantos produtos deseja cadastrar? ");
            scanf("%d", &qtd);

            if (qtd <= 0) {
                printf("Quantidade invalida.\n");
            } else if (totalProdutos + qtd > MAX_PRODUTOS) {
                printf("Espaco insuficiente! Restam %d vagas.\n",
                       MAX_PRODUTOS - totalProdutos);
            } else {
                /* FOR: repete o cadastro para cada produto */
                for (int i = 0; i < qtd; i++) {
                    int pos = totalProdutos;
                    int codigo;

                    printf("\n--- Produto %d ---\n", i + 1);

                    /* DO-WHILE: repete ate o codigo ser valido e unico */
                    do {
                        printf("Codigo (numero positivo): ");
                        scanf("%d", &codigo);

                        if (codigo <= 0) {
                            printf("Codigo invalido!\n");
                        } else if (buscarProduto(codigos, totalProdutos, codigo) != -1) {
                            printf("Codigo ja cadastrado!\n");
                            codigo = -1;
                        }
                    } while (codigo <= 0);
                    codigos[pos] = codigo;

                    printf("Nome: ");
                    scanf(" %29[^\n]", nomes[pos]);

                    do {
                        printf("Quantidade em estoque: ");
                        scanf("%d", &quantidades[pos]);
                        if (quantidades[pos] < 0) {
                            printf("A quantidade nao pode ser negativa!\n");
                        }
                    } while (quantidades[pos] < 0);

                    do {
                        printf("Preco unitario (R$): ");
                        scanf("%f", &precos[pos]);
                        if (precos[pos] <= 0) {
                            printf("O preco deve ser maior que zero!\n");
                        }
                    } while (precos[pos] <= 0);

                    totalProdutos++;
                    printf("Produto cadastrado com sucesso!\n");
                }
            }
            break;
        }

        case 2:
            printf("\n%-8s %-30s %-10s %-10s\n", "CODIGO", "NOME", "QTD", "PRECO");
            printf("------------------------------------------------------------\n");
            if (totalProdutos == 0) {
                printf("Nenhum produto cadastrado.\n");
            } else {
                for (int i = 0; i < totalProdutos; i++) {
                    printf("%-8d %-30s %-10d R$ %.2f", codigos[i], nomes[i],
                           quantidades[i], precos[i]);
                    if (quantidades[i] <= ESTOQUE_MINIMO) {
                        printf("  <-- ESTOQUE BAIXO");
                    }
                    printf("\n");
                }
            }
            break;

        case 3: {
            int codigo, qtd, pos;

            printf("\nCodigo do produto: ");
            scanf("%d", &codigo);
            pos = buscarProduto(codigos, totalProdutos, codigo);

            if (pos == -1) {
                printf("Produto nao encontrado!\n");
            } else {
                printf("Produto: %s | Disponivel: %d\n", nomes[pos], quantidades[pos]);
                printf("Quantidade a vender: ");
                scanf("%d", &qtd);

                if (qtd <= 0) {
                    printf("Quantidade invalida!\n");
                } else if (qtd > quantidades[pos]) {
                    printf("Estoque insuficiente!\n");
                } else {
                    float valor = qtd * precos[pos];
                    quantidades[pos] -= qtd;
                    faturamento += valor;
                    printf("Venda realizada! Total: R$ %.2f\n", valor);

                    if (quantidades[pos] <= ESTOQUE_MINIMO) {
                        printf("ATENCAO: estoque de %s esta baixo (%d un.)\n",
                               nomes[pos], quantidades[pos]);
                    }
                }
            }
            break;
        }

        case 4: {
            int codigo, qtd, pos;

            printf("\nCodigo do produto: ");
            scanf("%d", &codigo);
            pos = buscarProduto(codigos, totalProdutos, codigo);

            if (pos == -1) {
                printf("Produto nao encontrado!\n");
            } else {
                printf("Quantidade a repor: ");
                scanf("%d", &qtd);
                if (qtd > 0) {
                    quantidades[pos] += qtd;
                    printf("Novo estoque de %s: %d un.\n", nomes[pos], quantidades[pos]);
                } else {
                    printf("Quantidade invalida!\n");
                }
            }
            break;
        }

        case 5: {
            float valorEstoque = 0.0f;
            int   baixos = 0;
            int   posMaior = 0;

            if (totalProdutos == 0) {
                printf("\nNenhum produto cadastrado.\n");
            } else {
                for (int i = 0; i < totalProdutos; i++) {
                    valorEstoque += quantidades[i] * precos[i];
                    if (quantidades[i] <= ESTOQUE_MINIMO) {
                        baixos++;
                    }
                    if (quantidades[i] > quantidades[posMaior]) {
                        posMaior = i;
                    }
                }
                printf("\n===== RELATORIO GERAL =====\n");
                printf("Produtos cadastrados ....: %d\n", totalProdutos);
                printf("Valor total em estoque ..: R$ %.2f\n", valorEstoque);
                printf("Produtos com estoque baixo: %d\n", baixos);
                printf("Maior estoque ...........: %s (%d un.)\n",
                       nomes[posMaior], quantidades[posMaior]);
                printf("Faturamento das vendas ..: R$ %.2f\n", faturamento);
            }
            break;
        }

        case 0:
            printf("\nEncerrando o programa. Ate logo!\n");
            break;

        default:
            printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
