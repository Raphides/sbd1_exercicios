# include <stdio.h>
# include <stdlib.h>

/**
 * @brief tamanho máximo de NOME, CPF e PLACA respectivamente. Inicialmente valores padrões são inseridos.
 */
int limites[4] = {30, 12, 8};

typedef struct
{
    char* nome;
    char* cpf;
    
} Proprietario_do_Carro;


typedef struct 
{
    char* placa;
    Proprietario_do_Carro Proprietario_do_Carro;
} Carro;


void mostrar_titulo(char* titulo)
{
    printf("\n\n|=========== %s ===========|\n\n", titulo);
}

char* pedir_informacao_do_campo(char* campo, int tamanho_caracteres_da_resposta)
{
    char *informacao;
    printf("%s: ", campo);
    scanf("%s", informacao);
    return informacao;
}

void reconhecer_arquivo()
{
    FILE* arquivo;
    if (arquivo = fopen("dados.txt", "rb"))
    {
        fscanf("%c\n%c\n%c\n", limites, limites+1, limites+2);
    }
    else
    {
        fclose(arquivo);
        arquivo = fopen("dados.bin", "wb");
        for (size_t i = 0; i < 3; i++)
        {
            fputc(limites[i], arquivo);
            fputc('\n', arquivo);
        }
    }
    fclose(arquivo);
}



def cadastrar_proprietario(char* nome, char* cpf)

int main(int argc, char const *argv[])
{
    FILE* arquivo;
    unsigned char opcao;
    while (1)
    {
        printf("Bem vindo. O que deseja fazer?\n1.Registrar Proprietário de um Veículo\n2. Registrar Veículo\n3. Sair\nInsira uma opção: ");
        scanf("%c", &opcao);
        switch (opcao)
        {
        case '1':
            break;
        case '2':
            break;
        case '3':

            break;
        default:
            break;;
        } 
    }
    
    
    


    
    return 0;
}



