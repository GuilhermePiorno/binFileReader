// Este programa tem como objetivo auxiliar a leitura de arquivos binários quando
// tentando compreender e debugar programas.

#include <stdio.h>
#include <stdlib.h>
#include "tui.c"

#define CHAR 0;
#define INT 1;
#define FLOAT 2;
#define MAX_DECODE_HEADER_SIZE 100
#define MAX_FILE_NAME_SIZE 200




int main(void){
    clearScreen();
    int offset = 16; // (Bytes per row)
    int op, continua = 1;
    int nBlocks=1;
    int *dataTypes = malloc(sizeof(int));
    int grouping = 1;
    dataTypes[0] = CHAR;
    int offsetHex = 0; // 0 for hex, 1 for dec.
    char **DecodeHeaderNames = NULL;

    char nomeArquivo[MAX_FILE_NAME_SIZE];
    printf("\nPlease maximize or enlarge your window in order to better visualize your data.\n\n");
    printf("Decoded char data between 0x00 and 0x37 are going to be represented by a dot.\n");
    printf("Type the name of the binary file: ");
    scanf("%[^\n]s", nomeArquivo);
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "rb");

    while (!arquivo){
        clearScreen();
        printf("File not found!\nType the name of the binary file: ");
        scanf(" %[^\n]s", nomeArquivo);
        arquivo = fopen(nomeArquivo, "rb");
        
    }
    
    while(continua){
        clearScreen();
        hexdump(arquivo, offset, nomeArquivo, dataTypes, nBlocks, grouping, offsetHex, DecodeHeaderNames);
        printf("[1] Open file...\n");
        printf("[2] Adjust bytes per line displayed.\n");
        printf("[3] Decoder configuration.\n");
        printf("[4] Change byte group size.\n");
        printf("[5] Toggle offset display between Hex & Dec.\n");
        printf("[6] Set Decoder header.\n");
        printf("[9] Exit\n");
        scanf("%d", &op);
        clearScreen();
        hexdump(arquivo, offset, nomeArquivo, dataTypes, nBlocks, grouping, offsetHex, DecodeHeaderNames);

        switch(op){
            case 1:
                fclose(arquivo);
                arquivo = NULL;
                printf("Type the name of the binary file: ");
                scanf(" %[^\n]s", nomeArquivo);
                arquivo = fopen(nomeArquivo, "rb");
                while (!arquivo){
                    clearScreen();
                    printf("File not found!\nType the name of the binary file: ");
                    scanf(" %[^\n]s", nomeArquivo);
                    arquivo = fopen(nomeArquivo, "rb");
                }
                break;
            case 2:
                printf("Number of bytes per line: ");
                scanf("%d", &offset);
                break;
            case 3:
                printf("Data will be broken down into blocks, blocks will be interpreted as the given data types in the respective order.\n");
                printf("Number of blocks to break down data: ");
                scanf("%d", &nBlocks);
                free(dataTypes);
                dataTypes = malloc(nBlocks *sizeof(int));

                char readType[10];                
                
                for (int i=0; i<nBlocks; i++){
                    printf("What is the type of the %d%s data (int, float ou char): ", i+1, getSuffix(i+1));
                    scanf(" %s", readType);
                    dataTypes[i] = CHAR;
                    if (strcmp(readType, "char") == 0) dataTypes[i] = CHAR;
                    if (strcmp(readType, "int") == 0) dataTypes[i] = INT;
                    if (strcmp(readType, "float") == 0) dataTypes[i] = FLOAT;
                    
                }

                if (DecodeHeaderNames){
                    for (int i = 0; i<nBlocks; i++){
                        free(DecodeHeaderNames[i]);
                    }
                    free(DecodeHeaderNames);
                }
                DecodeHeaderNames = NULL;
                break;
            case 4:
                printf("GROUPING BYTES THAT WONT FIT THE LINE MAY CAUSE TABLE DISTORTIONS.\n");
                printf("Change how many bytes per lines are displayed accordingly to fix this.\n\n");
                printf("Type how many bytes should be grouped together: ");
                scanf("%d", &grouping);
                break;
            case 5:
                offsetHex = (offsetHex) ? 0 : 1;
                break;
            case 6:
                if (nBlocks > 16){
                    printf("Number of decoded blocks exceeds the maximum amount of headers supported (16).\n");
                    break;
                } 
                printf("The number of columns is set as the same as the number of decodable blocks.\n\n");
                printf("It's HIGHLY RECOMMENDED to setup byte grouping, decoding and number of bytes\n");
                printf("per line before adding headers to the decoded info.\n");
                if (DecodeHeaderNames){
                    for (int i = 0; i<nBlocks; i++){
                        free(DecodeHeaderNames[i]);
                    }
                    free(DecodeHeaderNames);
                }
                
                DecodeHeaderNames = malloc(sizeof(char*)*nBlocks);
                
                for (int i=0; i<nBlocks;i++){
                    char *vartype;
                    switch(dataTypes[i]){
                        case 0:
                            vartype = "char";
                            break;
                        case 1:
                            vartype = "int";
                            break;
                        case 2:
                            vartype = "float";
                            break;
                        default:
                            vartype = "char";
                    }

                    printf("Header %d title (%s): ", i+1, vartype);
                    DecodeHeaderNames[i] = malloc(sizeof(char)*MAX_DECODE_HEADER_SIZE);
                    scanf("  %[^\n]s", DecodeHeaderNames[i]);
                }
                break;
            default:
                continua = 0;
                printf("Exiting..\n");
                break;
        }

    }
    

    if (DecodeHeaderNames){
        for (int i = 0; i<nBlocks; i++){
            free(DecodeHeaderNames[i]);
        }
        free(DecodeHeaderNames);
    }
    free(dataTypes);
    fclose(arquivo);

    return 0;
}
