#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

unsigned int quantidade_proprietarios = 0;
typedef struct Veiculo_t{
    char placa[8];
    char modelo[20];
} Veiculo_t;

typedef struct Proprietario_t{
    char nome[30];
    char cpf[12];
    unsigned int num_carros;
    Veiculo_t *carros;
    struct Proprietario_t *proximo;
} Proprietario_t;


void transcrever_veiculo(Proprietario_t *proprietario, FILE* arquivo)
{
    for (unsigned int m = 0; m < proprietario->num_carros; m++)
    {
        fprintf(arquivo, "%s %s ", proprietario->carros[m].placa, proprietario->carros[m].modelo);
    }
}

void transcrever_proprietario(Proprietario_t* proprietario, FILE* arquivo)
{
    fprintf(arquivo, "%s %s %d ", proprietario->nome, proprietario->cpf, proprietario->num_carros);
    transcrever_veiculo(proprietario, arquivo);
}

int ler_veiculo(Proprietario_t *proprietario, FILE* arquivo)
{
    int scan_result;
    proprietario->carros = (Veiculo_t*) malloc(sizeof(Veiculo_t) * proprietario->num_carros);
    for (unsigned int n = 0; n < proprietario->num_carros; n++)
    {
        scan_result = fscanf(arquivo, "%s %s", proprietario->carros[n].placa, proprietario->carros[n].modelo);
        if (2 != scan_result)
        {
            free(proprietario->carros);
            return 0;
        }
    }
    return 1;
}

int ler_proprietario(Proprietario_t* proprietario, FILE* arquivo)
{
    int scan_result = fscanf(arquivo, "%s %s %d", proprietario->nome, proprietario->cpf, &(proprietario->num_carros));
    proprietario->proximo = NULL;
    if (scan_result == 3)
    {
        return ler_veiculo(proprietario, arquivo);
    }
    else if (scan_result == EOF)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void representar_veiculo(Proprietario_t *proprietario)
{
    for (unsigned int o = 0; o < proprietario->num_carros; o++)
    {
        printf("Carro %d:\n", o+1);
        printf("Placa: %s\n", proprietario->carros[o].placa);
        printf("Modelo: %s\n\n", proprietario->carros[o].modelo);
    }
}

void representar_proprietarios(Proprietario_t* proprietario)
{
    if (proprietario == NULL);
    else
    {
        printf("Nome: %s\n", proprietario->nome); 
        printf("CPF: %s\n\n", proprietario->cpf);
        representar_veiculo(proprietario);
        representar_proprietarios(proprietario->proximo);
    }
}

void desalocar_proprietarios_e_veiculos(Proprietario_t* proprietario)
{
    if (proprietario == NULL);
    else
    {
        free(proprietario->carros);
        desalocar_proprietarios_e_veiculos(proprietario->proximo);
        free(proprietario);
    }
}

char** linearizar_cpfs(Proprietario_t* proprietario_registrado_inicial)
{
    char** cpfs = (char**) malloc(sizeof(char*)*quantidade_proprietarios);
    for (unsigned int r = 0; r < quantidade_proprietarios;r++)
    {
        cpfs[r] = proprietario_registrado_inicial->cpf;
        proprietario_registrado_inicial = proprietario_registrado_inicial->proximo;
    }
    return cpfs;
    
}
// retorna -1 se a esqueda for maior, 1 se a direita for maior e 0 se ambos os cpfs forem iguais.
int comparar_cpf(char *esquerda, char *direita, unsigned int n_caracteres)
{
    int comparacao;
    for (unsigned int t = 0; t < n_caracteres; t++)
    {
        comparacao = esquerda[t] - direita[t];
        if (comparacao > 0)
        {
            return -1;
        }
        else if (comparacao < 0)
        {
            return 1;
        }   
    }
    return 0;
    
    
}


// retorna o index que o novo cpf deve ocupar. Caso seja repetido, retorna -1.
int busca_binaria_por_cpf(char** lista, char* cpf_alvo)
{
    int inicio = 0, fim = quantidade_proprietarios - 1; 
    int meio;
    int comparation;
    while (1)
    {
        if (inicio > fim)
        {
            break;
        }
        meio = (inicio + fim) / 2;
        comparation = comparar_cpf(lista[meio], cpf_alvo, 11);
        if (comparation < 0)
        {
            fim = meio - 1;
        }
        else if (comparation > 0)
        {
            inicio = meio + 1;
        }
        else
        {
            return -1;
        }
    }
    if (comparar_cpf(lista[meio], cpf_alvo, 11) == 0) {
        return -1;
    } else {
        return inicio;
    }
}



int inserir_por_cpf(Proprietario_t **proprietario_registrado_inicial, Proprietario_t* proprietario_novo)
{
    unsigned int index;
    Proprietario_t* proprietario_movido = NULL, *__tmp = NULL;
    if (proprietario_novo == NULL)
    {
        return 0;
    }
    else if ((*proprietario_registrado_inicial) == NULL)
    {
        (*proprietario_registrado_inicial) = proprietario_novo;
        return 1;
    }
    else
    {
        char** cpfs = linearizar_cpfs((*proprietario_registrado_inicial));
        if ((index = busca_binaria_por_cpf(cpfs, proprietario_novo->cpf)) == -1)
        {
            return 0;
        }
        else
        {
            if (index == 0)
            {
                __tmp = (*proprietario_registrado_inicial);
                (*proprietario_registrado_inicial) = proprietario_novo;
                (*proprietario_registrado_inicial)->proximo = __tmp;
            }
            else
            {
                proprietario_movido = *proprietario_registrado_inicial;
                for (unsigned int s = 0; s < index-1; s++)
                {
                    proprietario_movido = proprietario_movido->proximo;
                }
                __tmp = proprietario_movido->proximo;
                proprietario_movido->proximo = proprietario_novo;
                proprietario_novo->proximo = __tmp;
            }
            return 1;
        }
    }
}

int main() {
    FILE* arquivo;
    Proprietario_t* __finalizador = NULL; // variável interna para guardar a referência do último proprietário da lista.
    Proprietario_t* proprietario_registrado = NULL;
    Proprietario_t* proprietario_registrado_inicial = NULL;
    if (arquivo = fopen("dados.bin", "rb"))
    {
        proprietario_registrado = (Proprietario_t*) malloc(sizeof(Proprietario_t));
        proprietario_registrado_inicial = proprietario_registrado;
        while (1)
        {
            if (1 != ler_proprietario(proprietario_registrado, arquivo))
            {
                free(proprietario_registrado);
                if (proprietario_registrado == proprietario_registrado_inicial)
                {
                    proprietario_registrado_inicial = NULL;
                }
                else
                {
                    __finalizador->proximo = NULL;
                    __finalizador = NULL; //desvinculando o finalizador da tarefa
                }
                proprietario_registrado = NULL;
                break;
            }
            quantidade_proprietarios++;
            proprietario_registrado->proximo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
            __finalizador = proprietario_registrado;
            proprietario_registrado = proprietario_registrado->proximo;
        }
        fclose(arquivo);
    }



    Proprietario_t* proprietario_novo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
    while (1) {
        printf("Nome do Proprietário (ou 'sair' para finalizar): ");
        scanf("%s", proprietario_novo->nome);

        if (strcmp(proprietario_novo->nome, "sair") == 0) {
            free(proprietario_novo);
            proprietario_novo = NULL;
            break;
        }

        printf("CPF do Proprietário: ");
        scanf("%s", proprietario_novo->cpf);

        printf("Quantidade de carros do proprietário:");
        scanf("%d", &(proprietario_novo->num_carros));

        proprietario_novo->carros = (Veiculo_t*) malloc(sizeof(Veiculo_t) * proprietario_novo->num_carros);

        for (int j = 0; j < proprietario_novo->num_carros; j++) {
            printf("Placa do Carro %d:", j + 1);
            scanf("%s", proprietario_novo->carros[j].placa); 

            printf("Modelo do Carro %d:", j + 1);
            scanf("%s", proprietario_novo->carros[j].modelo); 
        }
        if (!(inserir_por_cpf(&proprietario_registrado_inicial, proprietario_novo)))
        {
            printf("O CPF inserido já existe no sistema. Por favor, digite outro ou relate o caso a um supervisor do sistema.\n\n");

            proprietario_novo->proximo = NULL;
            desalocar_proprietarios_e_veiculos(proprietario_novo);
        }
        proprietario_novo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
    }
    printf("\nPROPRIETÁRIOS CADASTRADOS\n\n");
    representar_proprietarios(proprietario_registrado_inicial);

    // transcrição no arquivos
    arquivo = fopen("dados.bin", "wb");
    proprietario_registrado = proprietario_registrado_inicial;
    while (proprietario_registrado != NULL)
    {
        transcrever_proprietario(proprietario_registrado, arquivo);
        proprietario_registrado = proprietario_registrado->proximo;
    }
    fclose(arquivo);
    printf("Dados salvos em 'dados.bin'\n\n");
    desalocar_proprietarios_e_veiculos(proprietario_registrado_inicial);
    return 0;
}
