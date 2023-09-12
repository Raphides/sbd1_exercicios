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

typedef struct Veiculo_t{
    char placa[7];
    char modelo[30];
    struct Veiculo_t *proximo;
} Veiculo_t;

typedef struct Proprietario_t{
    char nome[30];
    char cpf[11];
    struct Lista_Encadeada *carros;
    struct Proprietario_t *proximo;
} Proprietario_t;

void desalocar_veiculos(Proprietario_t* proprietario)
{
    if (proprietario == NULL || proprietario->carros == NULL || proprietario->carros->inicio == NULL);
    else
    {
        Veiculo_t* __finalizador, *__tempVeiculo;
        __tempVeiculo->proximo = NULL;
        __tempVeiculo = (Veiculo_t*) proprietario->carros->inicio;
        while (__tempVeiculo != NULL)
        {
            __finalizador = __tempVeiculo->proximo;
            free(__tempVeiculo);
            __tempVeiculo = __finalizador;
        }
    }
}

void desalocar_proprietarios_e_veiculos(Proprietario_t* proprietarioInicial)
{
    if (proprietarioInicial == NULL);
    else
    {
        desalocar_veiculos(proprietarioInicial);
        free(proprietarioInicial->carros);
        desalocar_proprietarios_e_veiculos(proprietarioInicial->proximo);
        free(proprietarioInicial);
    }
}

void transcrever_campo(char * campo, unsigned int tamanho_esperado)
{
    unsigned int tamanho_real = strlen(campo);
    if (tamanho_esperado <= tamanho_real);
    else
    {
        memset(campo+tamanho_real, ' ', tamanho_esperado - tamanho_real);
    }
}

void transcrever_proprietario(Proprietario_t* proprietario, FILE* arquivo)
{
    transcrever_campo(proprietario->nome, 30);
    fwrite(proprietario->nome, sizeof(char), 30, arquivo);
    transcrever_campo(proprietario->cpf, 11);
    fwrite(proprietario->cpf, sizeof(char), 11, arquivo);

    Veiculo_t* __tempVeiculo = proprietario->carros->inicio;
    for (unsigned int m = 0; m < proprietario->carros->tamanho; m++)
    {
        transcrever_campo(__tempVeiculo->placa, 7);
        fwrite(__tempVeiculo->placa, sizeof(char), 7, arquivo);
        transcrever_campo(__tempVeiculo->modelo, 30);
        fwrite(__tempVeiculo->modelo, sizeof(char), 30, arquivo);
        __tempVeiculo = __tempVeiculo->proximo;
    }
}

int ler_veiculo(Proprietario_t *proprietario, FILE* arquivo)
{
    int scanResult;
    proprietario->carros->inicio = (Veiculo_t*) malloc(sizeof(Veiculo_t));
    Veiculo_t* __tempVeiculo = (Veiculo_t*) proprietario->carros->inicio;
    scanResult = 0;
    scanResult += fread(__tempVeiculo->placa, sizeof(char), 7, arquivo);        
    scanResult += fread(__tempVeiculo->modelo, sizeof(char), 30, arquivo);
    if (37 != scanResult)
    {
        free(proprietario->carros);
        return 0;
    }
    for (unsigned int n = 1; n < proprietario->carros->tamanho; n++)
    {
        scanResult = 0;
        __tempVeiculo = __tempVeiculo->proximo;
        scanResult += fread(__tempVeiculo->placa, sizeof(char), 7, arquivo);        
        scanResult += fread(__tempVeiculo->modelo, sizeof(char), 30, arquivo);
        if (37 != scanResult)
        {
            desalocar_veiculos(proprietario);
            free(proprietario->carros);
            return 0;
        }
    }
    __tempVeiculo->proximo = NULL;
    return 1;
}

int ler_proprietario(Proprietario_t* proprietario, FILE* arquivo)
{
    proprietario->carros = (struct Lista_Encadeada*) malloc(sizeof(struct Lista_Encadeada));
    unsigned int scanResult = 0;
    scanResult += fread(proprietario->nome, sizeof(char), 30, arquivo);
    scanResult += fread(proprietario->cpf, sizeof(char), 11, arquivo);
    scanResult += fread(&(proprietario->carros->tamanho), sizeof(unsigned int), 1, arquivo);
    proprietario->proximo = NULL;
    if (scanResult == 42)
    {
        return ler_veiculo(proprietario, arquivo);
    }
    else
    {
        free(proprietario->carros);
        return 0;
    }
}

void representar_veiculo(Proprietario_t *proprietario)
{
    Veiculo_t* __tempVeiculo = (Veiculo_t*) proprietario->carros->inicio;
    for (unsigned int o = 0; o < proprietario->carros->tamanho; o++)
    {
        printf("Carro %d:\n", o+1);
        printf("Placa: %s\n", __tempVeiculo->placa);
        printf("Modelo: %s\n\n", __tempVeiculo->modelo);
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

        proprietarioNovo->carros = (struct Lista_Encadeada*) malloc(sizeof(struct Lista_Encadeada));

        printf("Quantidade de carros do proprietário:");
        scanf("%d", &(proprietarioNovo->carros->tamanho));

        if (proprietarioNovo->carros->tamanho < 1)
        {
            printf("Dado inválido. Por favor, refaça o seu cadastro.\n");
            free(proprietarioNovo->carros);
            continue;
        }

        Veiculo_t *veiculoNovo;
        Veiculo_t *__tempVeiculo;
        
        veiculoNovo = (Veiculo_t*) malloc(sizeof(Veiculo_t));
        printf("Placa do Carro 1:");
        scanf("%s", veiculoNovo->placa); 

        printf("Modelo do Carro 1:");
        scanf("%s", veiculoNovo->modelo);

        proprietarioNovo->carros->inicio = veiculoNovo;
        __tempVeiculo = veiculoNovo;

        for (int j = 1; j < proprietarioNovo->carros->tamanho; j++) {
            veiculoNovo = (Veiculo_t*) malloc(sizeof(Veiculo_t));
            printf("Placa do Carro %d:", j + 1);
            scanf("%s", veiculoNovo->placa); 

            printf("Modelo do Carro %d:", j + 1);
            scanf("%s", veiculoNovo->modelo);

            __tempVeiculo->proximo = veiculoNovo;
            __tempVeiculo = veiculoNovo;
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
