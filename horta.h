#define MAX_CLIENTES 100
#define MAX_EMAIL 80
#define MAX_SENHA 50
#define MAX_TELEFONE 20
#define MAX_NOME 50

typedef struct
{
    char nome[MAX_NOME];
    int quantidade;
    int test;
} Pedido;

typedef struct
{
    char login[MAX_EMAIL];
    char senha[MAX_SENHA];
    char telefone[MAX_TELEFONE];
} Usuario;

typedef struct
{
    char nome[MAX_NOME];
    float preco;
} Item;

int login(Usuario clientes[], int numClientes);
int verificarEmail(Usuario clientes[], int numClientes, const char email[]);
void registrarCliente(Usuario clientes[], int *numClientes);
void menuCliente();
void adicionarItem(Pedido produto[], int *numProdutos);
void editarItem(Pedido produto[], int numProduto);
void excluirItem(Pedido produto[], int *numProdutos);
void catalogoItens();
void carrinho();
void finalizarPedido();