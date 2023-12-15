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
    int nBlocks=1;
    int *dataTypes = malloc(sizeof(int));
    int grouping = 1;
    dataTypes[0] = CHAR;

    char nomeArquivo[131];
    printf("\nMaximize a janela para melhor visualizar seus dados!\n\n");
    printf("Dados decodificados para char entre 0x00 e 0x37 sao exibidos como \".\"\n");
    printf("Digite o nome do arquivo binario: ");
    scanf("%[^\n]s", nomeArquivo);
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "rb");

    while (!arquivo){
        limpaTela();
        printf("Arquivo nao encontrado!\nDigite o nome do arquivo binario: ");
        scanf(" %[^\n]s", nomeArquivo);
        arquivo = fopen(nomeArquivo, "rb");
        
    }
    
    while(continua){
        limpaTela();
        hexdump(arquivo, offset, nomeArquivo, dataTypes, nBlocks, grouping);
        printf("[0] Alterar arquivo\n");
        printf("[1] Mudar bytes por linha\n");
        printf("[2] Definir decoder\n");
        printf("[3] Agrupar bytes\n");
        printf("[9] Sair\n");
        scanf("%d", &op);

        switch(op){
            case 0:
                fclose(arquivo);
                arquivo = NULL;
                limpaTela();
                printf("Digite o nome do arquivo binario: ");
                scanf(" %[^\n]s", nomeArquivo);
                arquivo = fopen(nomeArquivo, "rb");
                while (!arquivo){
                    limpaTela();
                    printf("Arquivo nao encontrado!\nDigite o nome do arquivo binario: ");
                    scanf(" %[^\n]s", nomeArquivo);
                    arquivo = fopen(nomeArquivo, "rb");
                }
                break;
            case 1:
                limpaTela();
                hexdump(arquivo, offset, nomeArquivo, dataTypes, nBlocks, grouping);
                printf("Digite quantos bytes por linha: ");
                scanf("%d", &offset);
                break;
            case 2:
                limpaTela();
                hexdump(arquivo, offset, nomeArquivo, dataTypes, nBlocks, grouping);
                printf("Numero de dados para decodificar: ");
                scanf("%d", &nBlocks);
                free(dataTypes);
                dataTypes = malloc(nBlocks *sizeof(int));

                char readType[10];                
                
                for (int i=0; i<nBlocks; i++){
                    printf("Qual e o tipo do %d\xA7 dado (int, float ou char): ", i+1);
                    scanf(" %s", readType);
                    dataTypes[i] = CHAR;
                    if (strcmp(readType, "char") == 0) dataTypes[i] = CHAR;
                    if (strcmp(readType, "int") == 0) dataTypes[i] = INT;
                    if (strcmp(readType, "float") == 0) dataTypes[i] = FLOAT;
                    
                }
                break;
            case 3:
                limpaTela();
                hexdump(arquivo, offset, nomeArquivo, dataTypes, nBlocks, grouping);
                printf("AGRUPAR BYTES QUE NAO ENCAIXAM EM UMA LINHA PODE CAUSAR DISTORCOES NA TABELA\n");
                printf("Digite o tamanho dos grupos de bytes.\n");
                scanf("%d", &grouping);
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
