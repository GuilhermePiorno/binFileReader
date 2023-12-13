// Este programa tem como objetivo auxiliar a leitura de arquivos binários quando
// tentando compreender e debugar programas.
#include <stdio.h>
#include <stdlib.h>
#include "frame.c"


void limpaTela(void){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


int main(void){

    limpaTela();
    int offset = 16; // (Bytes per row)


    // printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 SYZ INVENTORY PROGRAM \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");

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

    int readStatus = 1, line = 0;
    unsigned char byte;

    // Name tab.
    horizontalFrameln(8, 201, 187);
    printf("\xBA %s \xBA\n", nomeArquivo);

    //Header
    horizontalFrame(8, 204, 202);
    horizontalFrameln(134, 0, 187);
    printf("\xBA Offset\t 00\t 01\t 02\t 03\t 04\t 05\t 06\t 07\t 08\t 09\t 0A\t 0B\t 0C\t 0D\t 0E\t 0F     \xBA\n");
    horizontalFrame(11, 204, 203);
    horizontalFrameln(131, 0, 185);
    
    //Actual table
    while(readStatus){
        printf("\xBA   %02X\t    \xBA    ", line);
        for (int byteNumber = line; byteNumber < (line+offset); byteNumber++){
            readStatus = fread(&byte, sizeof(unsigned char), 1, arquivo);

            if (readStatus){
                printf("0x%02X\t", byte);
            } 
            else{
                printf("\t");
            }
        }
        printf("\xBA\n");
        
        readStatus = fread(&byte, sizeof(unsigned char), 1, arquivo);
        fseek(arquivo, -1, SEEK_CUR);
        line += offset;
    }
    horizontalFrame(11, 200, 202);
    horizontalFrameln(131, 0, 188);
    fclose(arquivo);






    return 0;
}
