#include <stdio.h>
#include <stdlib.h>

#define tam_max 100

int qnt_pessoas = 0;

typedef struct Carro {
    char marca[tam_max];
    char modelo[tam_max];
    char placa[tam_max];
    int dono;
} Carro;

typedef struct Pessoa {
    char nome[tam_max];
    char cpf[tam_max];
    char tel[tam_max];
    int qnt_carros;
    struct Carro carros[tam_max];
} Pessoa;

void clear() // Limpa o Terminal
{
    #if defined(linux) || defined(unix) || defined(APPLE)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}


Pessoa *ler_arquivo(){
    FILE *file = fopen("arquivo.bin", "rb");

    fseek(file, 0, SEEK_END);

    qnt_pessoas = ftell(file)/sizeof(Pessoa);

    fseek(file, 0, SEEK_SET);

    Pessoa *pessoas = malloc(sizeof(Pessoa)*(qnt_pessoas));

    fread(pessoas, sizeof(Pessoa), qnt_pessoas, file);

    fclose(file);

    return pessoas;
}

void escrever_arquivo(Pessoa pessoa, int offset){

    FILE *file = fopen("arquivo.bin", "rb+");

    if(offset == -1) fseek(file, 0, SEEK_END);
    else fseek(file, offset, SEEK_SET);

    fwrite(&pessoa, sizeof(Pessoa), 1, file);

    fclose(file);

}

void cad_pessoa(){
    clear();
    getc(stdin);

    Pessoa pessoa;

    printf("    ----------------------------\n\
        CADASTRO PROPRIETARIO   \n\
    ----------------------------\n\
    Informe nome completo:\n    ");
    fgets(pessoa.nome, tam_max, stdin);

    printf("\n    Informe o cpf:\n    ");
    fgets(pessoa.cpf, tam_max, stdin);

    printf("\n    Informe o telefone:\n    ");
    fgets(pessoa.tel, tam_max, stdin);

    pessoa.qnt_carros = 0;

    escrever_arquivo(pessoa, -1);

}

void cad_carro(){
    clear();
    getc(stdin);

    Pessoa *pessoas = ler_arquivo();
    Carro carro;

    printf("    ----------------------------\n\
            CADASTRO CARRO      \n\
    ----------------------------\n\
    Informe o numero correspondente ao proprietario:\n    ");
    
    for(int i=0; i<qnt_pessoas; i++){
        printf("%d. %s\n    ", i+1, pessoas[i].nome);
    }
    scanf(" %d", &carro.dono);
    carro.dono--;

    getc(stdin);

    printf("\n    Informe a marca:\n    ");
    fgets(carro.marca, tam_max, stdin);

    printf("\n    Informe o modelo:\n    ");
    fgets(carro.modelo, tam_max, stdin);

    printf("\n    Informe a placa:\n    ");
    fgets(carro.placa, tam_max, stdin);

    pessoas[carro.dono].carros[pessoas[carro.dono].qnt_carros] = carro;
    
    pessoas[carro.dono].qnt_carros++;

    escrever_arquivo(pessoas[carro.dono], carro.dono*sizeof(Pessoa));

}


void lista_pessoas(){
    clear();
    
    Pessoa *pessoas = ler_arquivo();

    for(int i=0; i<qnt_pessoas; i++){
        printf("===========================================\n");
        printf("%d. Proprietario: %s\n    CPF: %s\n    Telefone: %s\n    Carro(s):\n", i+1, pessoas[i].nome, pessoas[i].cpf, pessoas[i].tel);
        
        for(int j = 0; j<pessoas[i].qnt_carros; j++){
            printf("\n        Marca: %s\n        Modelo: %s\n        Placa: %s", pessoas[i].carros[j].marca, pessoas[i].carros[j].modelo, pessoas[i].carros[j].placa);
            printf("    -----------------------------------\n");
        }
    }printf("==========================================\n");
}

void menu_inicial(){
    clear();

    int op;

    printf("    ----------------------------\n\
                MENU         \n\
    ----------------------------\n\
    1. Cadastrar Pessoa\n\
    2. Cadastrar Carro\n\
    3. Listar Pessoas\n\
    4. Finalizar Cadastro\n\
    ----------------------------\n\n\
    Digite a opção escolhida:\n\n    ");

    scanf("%d", &op);

    switch(op){
        case 1:
            cad_pessoa();
            menu_inicial();
            break;
        case 2:
            cad_carro();
            menu_inicial();
            break;
        case 3:
            lista_pessoas();

            printf("\n\n Digite qualquer tecla para voltar ao menu...\n\n");
            if(scanf(" %*s") != EOF) menu_inicial();
            
            break;
        case 4:
            lista_pessoas();
            break;
    }
    
    
}


int main() {
    if(fopen("arquivo.bin", "rb") == NULL) {
        FILE *file = fopen("arquivo.bin", "wb");
        fclose(file);
    }
    
    menu_inicial();
    return 0;
}