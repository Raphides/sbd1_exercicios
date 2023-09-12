/**
 * @file aula1exer1_RaphaelSilva_211039690.c
 * @author Raphael Mendes da Silva (211039690)
 * @brief Programa de cadastro de proprietários e carros.
 * 
 * Como esta atividade foi entregue em atraso, foi dito para reescrever um programa de algum colega. Portanto, reescrevi o programa do Jefferson Oliveira, presente nos arquivos da turma.
 * @version 0.1
 * @date 2023-09-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Veiculo_t{
    char placa[7];
    char modelo[20];
} Veiculo_t;

typedef struct Proprietario_t{
    char nome[30];
    char cpf[11];
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

void representar_proprietario(Proprietario_t* proprietario)
{
    if (proprietario == NULL);
    else
    {
        printf("Nome: %s\n", proprietario->nome); 
        printf("CPF: %s\n\n", proprietario->cpf);
        representar_veiculo(proprietario);
        representar_proprietario(proprietario->proximo);
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

int main() {
    FILE* arquivo;
    Proprietario_t* proprietario_novo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
    Proprietario_t* __finalizador = NULL; // variável interna para guardar a referência do último proprietário da lista.
    Proprietario_t* proprietario_novo_inicial = proprietario_novo;
    while (1) {
        proprietario_novo->proximo = NULL;
        printf("Nome do Proprietário (ou 'sair' para finalizar): ");
        scanf("%s", proprietario_novo->nome);

        if (strcmp(proprietario_novo->nome, "sair") == 0) {
            free(proprietario_novo);
            if (proprietario_novo == proprietario_novo_inicial)
            {
                proprietario_novo_inicial = NULL;
            }
            else
            {
                __finalizador->proximo = NULL;
                __finalizador = NULL; // desvinculando o finalizador da tarefa.
            }
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
        proprietario_novo->proximo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
        __finalizador = proprietario_novo;
        proprietario_novo = proprietario_novo->proximo;
    }


    Proprietario_t* proprietario_registrado = NULL;
    Proprietario_t* proprietario_registrado_inicial = NULL;
    printf("\nPROPRIETÁRIOS CADASTRADOS\n\n");
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
            proprietario_registrado->proximo = (Proprietario_t*) malloc(sizeof(Proprietario_t));
            __finalizador = proprietario_registrado;
            proprietario_registrado = proprietario_registrado->proximo;
        }
        representar_proprietario(proprietario_registrado_inicial);
        fclose(arquivo);
    }
    representar_proprietario(proprietario_novo_inicial);

    // transcrição no arquivos
    arquivo = fopen("dados.bin", "ab");
    proprietario_novo = proprietario_novo_inicial;
    while (proprietario_novo != NULL)
    {
        transcrever_proprietario(proprietario_novo, arquivo);
        proprietario_novo = proprietario_novo->proximo;
    }
    fclose(arquivo);
    printf("Dados salvos em 'dados.bin'\n\n");
    desalocar_proprietarios_e_veiculos(proprietario_novo_inicial);
    desalocar_proprietarios_e_veiculos(proprietario_registrado_inicial);
    return 0;
}
