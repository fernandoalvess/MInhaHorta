#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "horta.h"

int login(Usuario clientes[], int numClientes)
{
    char login[MAX_EMAIL];
    char senha[MAX_SENHA];

    printf("MINHA HORTA\n");
    printf("Login: ");
    scanf("%s", login);
    printf("Senha: ");
    scanf("%s", senha);

    for (int i = 0; i < numClientes; i++)
    {
        if (strcmp(login, clientes[i].login) == 0 && strcmp(senha, clientes[i].senha) == 0)
        {
            return i; // cliente logado
        }
    }

    return -1; // se nao encontrar um cliente com o login e senha informados
}

int verificarEmail(Usuario clientes[], int numClientes, const char email[])
{
    // Verifica se o e-mail já está cadastrado no sistema
    for (int i = 0; i < numClientes; i++)
    {
        if (strcmp(email, clientes[i].login) == 0)
        {
            return 0; // e-mail já existe
        }
    }

    // Verifica se o e-mail é válido
    if (strstr(email, "@") != NULL && strlen(email) < MAX_EMAIL)
    {
        return 1; // é válido e único
    }
    else
    {
        return -1; // o e-mail é inválido
    }
}

void registrarCliente(Usuario clientes[], int *numClientes)
{
    char email[MAX_EMAIL];
    char senha[MAX_SENHA];
    char telefone[MAX_TELEFONE];

    if (*numClientes >= MAX_CLIENTES)
    {
        printf("Limite de clientes atingido. Impossivel cadastrar mais usuarios.\n");
        return;
    }

    do
    {
        printf("Cadastre-se\n");
        printf("Email: (Ex: cliente@minhahorta.com)\n");
        scanf("%s", email);

        // Verificar se o e-mail já está registrado
        int emailExistente = verificarEmail(clientes, *numClientes, email);
        if (emailExistente == 0)
        {
            printf("Este e-mail ja esta cadastrado. Tente outro.\n");
        }
        else if (emailExistente == -1)
        {
            printf("Email invalido. Tente novamente.\n");
        }
    } while (verificarEmail(clientes, *numClientes, email) != 1);

    printf("Senha: ");
    scanf("%s", senha);
    printf("Telefone: ");
    scanf("%s", telefone);

    FILE *arquivo = fopen("cliente.txt", "a");
    if (arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!\n");
        return;
    }

    fprintf(arquivo, "%s %s %s\n", email, senha, telefone);

    fclose(arquivo);

    // Atualiza o array de clientes
    strcpy(clientes[*numClientes].login, email);
    strcpy(clientes[*numClientes].senha, senha);
    strcpy(clientes[*numClientes].telefone, telefone);
    (*numClientes)++;
}

void menuCliente()
{

    printf("Menu de Opcoes:\n");
    printf("1 - Catalago dos itens\n");
    printf("2 - Adicionar itens\n");
    printf("3 - Mostrar carrinho de compras\n");
    printf("4 - Editar item\n");
    printf("5 - Excluir item\n");
    printf("6 - Finalizar pedido\n");
    printf("0 - Sair da minha conta\n");
    printf("Escolha uma opcao:\n");
}

void adicionarItem(Pedido produto[], int *numProdutos)
{
    printf("Boas compras!!!\n");
    printf("Informe o nome do produto:\n");
    scanf("%s", produto[*numProdutos].nome);
    printf("Informe a quantidade:\n");
    scanf("%d", &produto[*numProdutos].quantidade);

    FILE *arquivo = fopen("pedido.txt", "a");
    if (arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!");
        return;
    }

    fprintf(arquivo, "%d %s\n", produto[*numProdutos].quantidade, produto[*numProdutos].nome);
    fclose(arquivo);

    (*numProdutos)++;
    printf("Item adicionado com sucesso!\n");
}

void editarItem(Pedido produto[], int numProduto)
{
    FILE *arquivo;
    arquivo = fopen("pedido.txt", "r+");
    if (arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!\n");
        exit(1);
    }

    char nome[MAX_NOME];
    printf("Digite o nome do produto:\n");
    scanf("%s", nome);

    int i = 0;
    int produtoEncontrado = 0;
    for (i = 0; i < numProduto; i++)
    {
        if (strcmp(nome, produto[i].nome) == 0)
        {
            printf("Informe a nova quantidade:\n");
            scanf("%d", &produto[i].quantidade);
            printf("Produto editado com sucesso!\n");
            produtoEncontrado = 1;
        }
        fprintf(arquivo, "%d %s\n", produto[i].quantidade, produto[i].nome);
    }

    fclose(arquivo);

    if (!produtoEncontrado)
    {
        printf("Produto nao encontrado!\n");
    }
}

void excluirItem(Pedido produto[], int *numProdutos)
{
    FILE *arquivo = fopen("pedido.txt", "r+");
    if (arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!\n");
        exit(1);
    }

    char nome[MAX_NOME];
    printf("Digite o nome do produto que deseja excluir:\n");
    scanf("%s", nome);

    int i;
    int produtoEncontrado = 0;

    FILE *temp = tmpfile(); // Arquivo temporário em memória
    if (temp == NULL)
    {
        printf("Erro ao criar arquivo temporário!\n");
        exit(1);
    }

    for (i = 0; i < *numProdutos; i++)
    {
        if (strcmp(nome,
                   produto[i].nome) == 0)
        {
            produtoEncontrado = 1;
            continue;
        }
        fprintf(temp, "%d %s\n", produto[i].quantidade, produto[i].nome);
    }

    if (!produtoEncontrado)
    {
        printf("Produto nao encontrado!\n");
        fclose(temp);
        fclose(arquivo);
        return;
    }

    // Voltar para o início do arquivo original
    rewind(temp);

    // Limpar o arquivo original
    freopen("pedido.txt", "w", arquivo);

    // Copiar os dados do arquivo temporário para o arquivo original
    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), temp) != NULL)
    {
        fputs(buffer, arquivo);
    }

    fclose(temp);
    fclose(arquivo);

    printf("Produto excluido com sucesso!\n");
    (*numProdutos)--;
}

void catalogoItens()
{
    Item frutas[15] = {
        {"Banana", 2.0}, {"Maca", 3.5}, {"Laranja", 2.8}, {"Abacaxi", 4.0}, {"Melancia", 5.0}, {"Uva", 6.0}, {"Manga", 3.2}, {"Morango", 8.0}, {"Pera", 3.0}, {"Limao", 2.5}, {"Caju", 4.5}, {"Goiaba", 3.0}, {"Maracuja", 4.5}, {"Pessego", 4.0}, {"Kiwi", 6.5}};

    Item legumes[15] = {
        {"Tomate", 3.0}, {"Cenoura", 2.5}, {"Batata", 2.0}, {"Cebola", 2.8}, {"Alho", 4.0}, {"Pimentao", 3.5}, {"Abobora", 3.0}, {"Brocolis", 5.0}, {"Couve", 3.5}, {"Quiabo", 4.0}, {"Berinjela", 3.0}, {"Repolho", 2.5}, {"Chuchu", 2.0}, {"Beterraba", 2.8}, {"Espinafre", 4.0}};

    Item hortalicas[15] = {
        {"Alface", 2.0}, {"Cenoura", 1.5}, {"Tomate", 2.0}, {"Cebola", 1.8}, {"Pimentao", 2.5}, {"Abobrinha", 2.0}, {"Batata", 1.5}, {"Repolho", 1.8}, {"Couve", 2.2}, {"Alho", 2.0}, {"Beterraba", 2.0}, {"Brocolis", 2.5}, {"Rucula", 3.0}, {"Escarola", 2.5}, {"Agriao", 2.0}};

    printf("Frutas disponiveis:\n");
    for (int i = 0; i < 15; i++)
    {
        printf("%-15s| R$ %.2f (kg)\t", frutas[i].nome, frutas[i].preco);
        if ((i + 1) % 3 == 0) // Quebra de linha após imprimir 3 itens
            printf("\n");
    }

    printf("\nLegumes disponiveis:\n");
    for (int i = 0; i < 15; i++)
    {
        printf("%-15s|R$ %.2f (kg)\t", legumes[i].nome, legumes[i].preco);
        if ((i + 1) % 3 == 0)
            printf("\n");
    }

    printf("\nHortalicas disponiveis:\n");
    for (int i = 0; i < 15; i++)
    {
        printf("%-15s| R$ %.2f (kg)\t", hortalicas[i].nome, hortalicas[i].preco);
        if ((i + 1) % 3 == 0)
            printf("\n");
    }
}

void carrinho(Pedido produtos[], int numProdutos)
{
    FILE *arquivo = fopen("pedido.txt", "r");
    if (arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!\n");
        return;
    }

    // Lê os produtos do arquivo e armazena-os em um array
    int i = 0;
    while (fscanf(arquivo, "%d %s", &produtos[i].quantidade, produtos[i].nome) == 2 && i < numProdutos)
    {
        i++;
    }
    fclose(arquivo);

    ordenarCarrinho(produtos, numProdutos);

    printf("Carrinho de Compras:\n");
    printf("---------------------------------------------------\n");
    printf("  %-20s | %-10s\n", "Nome do Produto", "Quantidade");
    printf("---------------------------------------------------\n");

    int itensNoCarrinho = 0;
    for (int j = 0; j < numProdutos; j++)
    {
        printf("  %-20s | %-10d\n", produtos[j].nome, produtos[j].quantidade);
        itensNoCarrinho++;
    }

    if (itensNoCarrinho == 0)
    {
        printf("Seu carrinho esta vazio.\n");
    }

    printf("---------------------------------------------------\n");
}


void finalizarPedido()
{
    char formaPagamento;

    printf("Escolha a forma de pagamento (P - Pix, C - Cartao): ");
    scanf(" %c", &formaPagamento);
    while (formaPagamento != 'P' && formaPagamento != 'C')
    {
        printf("Opcao invalida. Por favor, escolha P para Pix ou C para Cartao: ");
        scanf(" %c", &formaPagamento);
    }
    system("cls");
    
    if (formaPagamento == 'P')
    {
        printf("Voce selecionou Pix como forma de pagamento.\n");
    }
    else
    {
        printf("Voce selecionou Cartao como forma de pagamento.\n");
    }

    // Exclui todos os dados do arquivo "pedido.txt"
    FILE *arquivo = fopen("pedido.txt", "w"); // Abrir o arquivo no modo de escrita, o que limpa seu conteudo
    if (arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!\n");
        return;
    }
    fclose(arquivo);

    printf("Seu pedido esta sendo separado e pesado!\nEm 10 minutos entraremos em contato pelo WhatsApp\npara passar o valor e coletar endereco da entrega!\n");
    system("pause");
    system("cls");
    printf("Obrigado por escolher MINHA HORTA, volte sempre!");
    printf("\n");
    system("pause");
    system("cls");
}

// Funcao de comparacao para a ordenacao dos itens do carrinho pelo nome
int compararNomes(const void *a, const void *b)
{
    const Pedido *pedidoA = (const Pedido *)a;
    const Pedido *pedidoB = (const Pedido *)b;
    return strcmp(pedidoA->nome, pedidoB->nome);
}

// Funcao para ordenar os itens do carrinho
void ordenarCarrinho(Pedido produtos[], int numProdutos)
{
    qsort(produtos, numProdutos, sizeof(Pedido), compararNomes);
}