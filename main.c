#include "horta.c"

int main()
{
    Usuario clientes[MAX_CLIENTES];
    Pedido produtos[100];
    int numClientes = 0;
    int numProdutos = 0;
    int indiceClienteLogado = -1; // Para armazenar o índice do cliente logado

    FILE *arquivoClientes = fopen("cliente.txt", "r");
    if (arquivoClientes != NULL)
    {
        while (fscanf(arquivoClientes, "%s %s %s", clientes[numClientes].login, clientes[numClientes].senha, clientes[numClientes].telefone) != EOF)
        {
            numClientes++;
            if (numClientes >= MAX_CLIENTES)
            {
                printf("Limite maximo de clientes atingido.\n");
                break;
            }
        }
        fclose(arquivoClientes);
    }

    int opcaoLogin;
    do
    {
        printf("MINHA HORTA");
        printf("\n1 - Entrar\n2 - Cadastrar\n0 - Sair\n");
        printf("Escolha uma opcao:\n");
        scanf("%d", &opcaoLogin);
        system("cls");

        switch (opcaoLogin)
        {
        case 1:
            indiceClienteLogado = login(clientes, numClientes);
            if (indiceClienteLogado != -1)
            {
                system("cls");
                printf("Login bem-sucedido!\nBem vindo(a) ao Minha Horta\n");
                system("pause");
                int opcaoCliente;
                do
                {
                    system("cls");
                    menuCliente();
                    scanf("%d", &opcaoCliente);

                    switch (opcaoCliente)
                    {
                    case 1:
                        system("cls");
                        catalogoItens();
                        system("pause");
                        break;
                    case 2:
                        system("cls");
                        adicionarItem(produtos, &numProdutos);
                        system("pause");
                        break;
                    case 3:
                        system("cls");
                        carrinho(produtos, numProdutos);
                        system("pause");
                        break;
                    case 4:
                        system("cls");
                        editarItem(produtos, numProdutos);
                        system("pause");
                        break;
                    case 5:
                        system("cls");
                        excluirItem(produtos, &numProdutos);
                        system("pause");
                        break;
                    case 6:
                        system("cls");
                        finalizarPedido();
                        break;
                    case 0:
                        system("cls");
                        printf("Saindo da sua conta.\n");
                        break;
                    default:
                        printf("Opcao invalida.\n");
                    }
                } while (opcaoCliente != 0);
            }
            else
            {
                system("cls");
                printf("Login ou senha incorretos.\n");
            }
            break;
        case 2:
            registrarCliente(clientes, &numClientes);
            system("cls");
            printf("Cliente registrado com sucesso!\n");
            break;
        case 0:
            system("cls");
            printf("Saindo da Minha Horta, volte sempre!\n");
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (opcaoLogin != 0);

    return 0;
}