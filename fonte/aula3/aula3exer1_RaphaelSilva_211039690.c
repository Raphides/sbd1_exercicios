/**
 * @file aula2exer1_RaphaelSilva_211039690.c
 * @author Raphael Mendes da Silva (211039690)
 * @brief Programa de cadastro de proprietários e carros. Os dados são organizados em ordem crescente baseado no CPF de cada proprietário.
 * 
 * Como a atividade 1 foi entregue em atraso, foi dito para reescrever um programa de algum colega. Portanto, reescrevi o programa do Jefferson Oliveira, presente nos arquivos da turma.
 * @version 0.3
 * @date 03/09/2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Lista_Encadeada
{
    unsigned int tamanho;
    void *inicio;
};


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

void representar_proprietarios(Proprietario_t* proprietarioInicial)
{
    if (proprietarioInicial == NULL);
    else
    {
        printf("Nome: %s\n", proprietarioInicial->nome); 
        printf("CPF: %s\n\n", proprietarioInicial->cpf);
        representar_veiculo(proprietarioInicial);
        representar_proprietarios(proprietarioInicial->proximo);
    }
}

void desalocar_proprietarios_e_veiculos(Proprietario_t* proprietarioInicial)
{
    if (proprietarioInicial == NULL);
    else
    {
        free(proprietarioInicial->carros);
        desalocar_proprietarios_e_veiculos(proprietarioInicial->proximo);
        free(proprietarioInicial);
    }
}

char** linearizar_cpfs(struct Lista_Encadeada* listaProprietarios)
{
    Proprietario_t *__tempRegistro = (Proprietario_t*) listaProprietarios->inicio;
    char** cpfs = (char**) malloc(sizeof(char*)*listaProprietarios->tamanho);
    for (unsigned int r = 0; r < listaProprietarios->tamanho;r++)
    {
        cpfs[r] = __tempRegistro->cpf;
        __tempRegistro = __tempRegistro->proximo;
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
int busca_binaria_por_cpf(char** lista, char* cpfAlvo, int indexFim)
{
    int indexInicio = 0; 
    int meio;
    int comparation;
    while (1)
    {
        if (indexInicio > indexFim)
        {
            break;
        }
        meio = (indexInicio + indexFim) / 2;
        comparation = comparar_cpf(lista[meio], cpfAlvo, 11);
        if (comparation < 0)
        {
            indexFim = meio - 1;
        }
        else if (comparation > 0)
        {
            indexInicio = meio + 1;
        }
        else
        {
            return -1;
        }
    }
    if (comparar_cpf(lista[meio], cpfAlvo, 11) == 0) {
        return -1;
    } else {
        return indexInicio;
    }
}



int inserir_por_cpf(struct Lista_Encadeada *listaProprietarios, Proprietario_t* proprietarioNovo)
{
    unsigned int index;
    Proprietario_t* proprietario_movido = NULL, *__tmp = NULL;
    if (proprietarioNovo == NULL || listaProprietarios == NULL)
    {
        return 0;
    }
    else if (listaProprietarios->inicio == NULL || listaProprietarios->tamanho == 0)
    {
        listaProprietarios->inicio = proprietarioNovo;
        return 1;
    }
    else
    {
        char** cpfs = linearizar_cpfs(listaProprietarios);
        if ((index = busca_binaria_por_cpf(cpfs, proprietarioNovo->cpf, (listaProprietarios->tamanho)-1)) == -1)
        {
            return 0;
        }
        else
        {
            if (index == 0)
            {
                __tmp = (Proprietario_t*) listaProprietarios->inicio;
                listaProprietarios->inicio = proprietarioNovo;
                ((Proprietario_t*) listaProprietarios->inicio)->proximo = __tmp;
            }
            else
            {
                proprietario_movido = (Proprietario_t*) listaProprietarios->inicio;
                for (unsigned int s = 0; s < index-1; s++)
                {
                    proprietario_movido = proprietario_movido->proximo;
                }
                __tmp = proprietario_movido->proximo;
                proprietario_movido->proximo = proprietarioNovo;
                proprietarioNovo->proximo = __tmp;
            }
            listaProprietarios->tamanho++;
            return 1;
        }
    }
}

int main() {
    FILE* arquivo;
    struct Lista_Encadeada *listaProprietarios = (struct Lista_Encadeada*) malloc(sizeof(struct Lista_Encadeada));
    listaProprietarios->tamanho = 0;
    listaProprietarios->inicio = NULL;
    Proprietario_t* __finalizador = NULL; // variável interna para guardar a referência do último proprietário da lista.
    Proprietario_t* proprietarioRegistrado = NULL;
    if (arquivo = fopen("dados.bin", "rb"))
    {
        listaProprietarios->inicio = malloc(sizeof(Proprietario_t));
        proprietarioRegistrado = (Proprietario_t*) listaProprietarios->inicio;
        while (1)
        {
            if (1 != ler_proprietario(proprietarioRegistrado, arquivo))
            {
                if (proprietarioRegistrado == (Proprietario_t*) listaProprietarios->inicio)
                {
                    listaProprietarios->inicio = NULL;
                }
                else
                {
                    __finalizador->proximo = NULL;
                    __finalizador = NULL; //desvinculando o finalizador da tarefa
                }
                free(proprietarioRegistrado);
                proprietarioRegistrado = NULL;
                break;
            }
            listaProprietarios->tamanho++;
            proprietarioRegistrado->proximo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
            __finalizador = proprietarioRegistrado;
            proprietarioRegistrado = proprietarioRegistrado->proximo;
        }
        fclose(arquivo);
    }



    Proprietario_t* proprietarioNovo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
    while (1) {
        printf("Nome do Proprietário (ou 'sair' para finalizar): ");
        scanf("%s", proprietarioNovo->nome);

        if (strcmp(proprietarioNovo->nome, "sair") == 0) {
            free(proprietarioNovo);
            proprietarioNovo = NULL;
            break;
        }

        printf("CPF do Proprietário: ");
        scanf("%s", proprietarioNovo->cpf);

        printf("Quantidade de carros do proprietário:");
        scanf("%d", &(proprietarioNovo->num_carros));

        proprietarioNovo->carros = (Veiculo_t*) malloc(sizeof(Veiculo_t) * proprietarioNovo->num_carros);

        for (int j = 0; j < proprietarioNovo->num_carros; j++) {
            printf("Placa do Carro %d:", j + 1);
            scanf("%s", proprietarioNovo->carros[j].placa); 

            printf("Modelo do Carro %d:", j + 1);
            scanf("%s", proprietarioNovo->carros[j].modelo); 
        }
        if (!(inserir_por_cpf(listaProprietarios, proprietarioNovo)))
        {
            printf("O CPF inserido já existe no sistema. Por favor, digite outro ou relate o caso a um supervisor do sistema.\n\n");

            proprietarioNovo->proximo = NULL;
            desalocar_proprietarios_e_veiculos(proprietarioNovo);
        }
        proprietarioNovo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
    }
    printf("\nPROPRIETÁRIOS CADASTRADOS\n\n");
    representar_proprietarios((Proprietario_t*) listaProprietarios->inicio);

    // transcrição no arquivos
    arquivo = fopen("dados.bin", "wb");
    proprietarioRegistrado = (Proprietario_t*)listaProprietarios->inicio;
    while (proprietarioRegistrado != NULL)
    {
        transcrever_proprietario(proprietarioRegistrado, arquivo);
        proprietarioRegistrado = proprietarioRegistrado->proximo;
    }
    fclose(arquivo);
    printf("Dados salvos em 'dados.bin'\n\n");
    desalocar_proprietarios_e_veiculos((Proprietario_t*) listaProprietarios->inicio);
    free(listaProprietarios);
    return 0;
}
