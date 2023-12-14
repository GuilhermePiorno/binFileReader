// Este programa tem como objetivo auxiliar a leitura de arquivos binários quando
// tentando compreender e debugar programas.

#include <stdio.h>
#include <stdlib.h>
#include "tui.c"

#define CHAR 0;
#define INT 1;
#define FLOAT 2;




int main(void){
    limpaTela();
    int offset = 16; // (Bytes per row)
    int op, continua = 1;
    int nBlocks;

    char nomeArquivo[131];
    printf("Digite o nome do arquivo binario: ");
    scanf("%[^\n]s", nomeArquivo);
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "rb");

    limpaTela();
    while (!arquivo){
        printf("Arquivo nao encontrado!\nDigite o nome do arquivo binario: ");
        scanf(" %[^\n]s", nomeArquivo);
        arquivo = fopen(nomeArquivo, "rb");
        limpaTela();
    }
    
    hexdump(arquivo, offset, nomeArquivo);

    while(continua){
        printf("[1] Mudar bytes por linha\n");
        printf("[2] Definir decoder\n");
        printf("[3] Agregar bytes\n");
        printf("[9] Sair\n");
        scanf("%d", &op);

        switch(op){
            case 1:
                limpaTela();
                hexdump(arquivo, offset, nomeArquivo);
                printf("Digite quantos bytes por linha: ");
                scanf("%d", &offset);
                limpaTela();
                hexdump(arquivo, offset, nomeArquivo);
                break;
            case 2:
                limpaTela();
                hexdump(arquivo, offset, nomeArquivo);
                printf("Numero de dados para decodificar: ");
                scanf("%d", &nBlocks);
                int *dataTypes;
                char readType[10];
                dataTypes = malloc(nBlocks *sizeof(int));
                for (int i=0; i<nBlocks; i++){
                    printf("Qual e o tipo do %d\xA7 dado (int, float ou char): ", i+1);
                    scanf(" %s", readType);
                    dataTypes[i] = CHAR;
                    if (strcmp(readType, "char") == 0) dataTypes[i] = CHAR;
                    if (strcmp(readType, "int") == 0) dataTypes[i] = INT;
                    if (strcmp(readType, "float") == 0) dataTypes[i] = FLOAT;
                }
                limpaTela();
                hexdumpWithDecode(arquivo, offset, nomeArquivo, dataTypes, nBlocks);
                break;
            case 3:
                limpaTela();
                hexdump(arquivo, offset, nomeArquivo);
                printf("Funcao nao implementada.\n");
                break;
            default:
                continua = 0;
                printf("Saindo..\n");
                break;
        }
    }
    












    fclose(arquivo);






    return 0;
}
