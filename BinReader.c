// Este programa tem como objetivo auxiliar a leitura de arquivos binários quando
// tentando compreender e debugar programas.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tui.c"
#include "misc.c"

#define CHAR 0;
#define INT 1;
#define FLOAT 2;
#define MAX_DECODE_HEADER_SIZE 100
#define MAX_FILE_NAME_SIZE 200




int main(void){
    clearScreen();
    int bytesPerLine = 16; // (Bytes per row. Offset)
    int op, continua = 1;
    int nBlocks=1;
    int *dataTypes = malloc(sizeof(int));
    int grouping = 1;
    int *groupingArray = NULL;
    dataTypes[0] = CHAR;
    int offsetHex = 0; // 0 for hex, 1 for dec.
    char **DecodeHeaderNames = NULL;
    int decoderSetupStatus = 0;
    int bytesPerLinePrediction;
    int bytesPerDataType;
    char groupingStr[100]; // Actual max size should be 2*bytesperline to cover the case "1 1 ... 1"
    int lastpos, groupingByteSum;

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
        hexdump(arquivo, bytesPerLine, nomeArquivo, dataTypes, nBlocks, grouping, offsetHex, DecodeHeaderNames);
        printf("[1] Open file...\n");
        printf("[2] Adjust bytes per line displayed.\n");
        printf("[3] Decoder configuration.\n");
        printf("[4] Change byte group size.\n");
        printf("[5] Toggle offset display between Hex & Dec.\n");
        printf("[6] Set Decoder header.\n");
        printf("[9] Exit\n");
        scanf("%d", &op);
        clearScreen();
        hexdump(arquivo, bytesPerLine, nomeArquivo, dataTypes, nBlocks, grouping, offsetHex, DecodeHeaderNames);

        switch(op){
            case 1: // OPEN FILE.
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
                decoderSetupStatus = 0;
                break;
            case 2: // BYTES PER LINE.
                printf("Number of bytes per line: ");
                scanf("%d", &bytesPerLine);
                break;
            case 3: // DECODER SETUP
                bytesPerLinePrediction = 0;
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
                    bytesPerDataType = 1;
                    if (strcmp(readType, "char") == 0){
                        dataTypes[i] = CHAR;
                        bytesPerDataType = 1;
                    } 
                    if (strcmp(readType, "int") == 0){
                        dataTypes[i] = INT;
                        bytesPerDataType = 4;
                    } 

                    if (strcmp(readType, "float") == 0){
                        dataTypes[i] = FLOAT;
                        bytesPerDataType = 4;
                    } 
                    bytesPerLinePrediction += bytesPerDataType;
                }

                char autoAdjustOption;
                if (bytesPerLinePrediction != bytesPerLine){
                    printf("Do you want to auto-adjust the number of bytes per line accordingly? (y/n): ");
                    scanf(" %c", &autoAdjustOption);
                    autoAdjustOption = tolower(autoAdjustOption);
                    printf("%c", autoAdjustOption);
                    if (autoAdjustOption == 'y'){
                        bytesPerLine = bytesPerLinePrediction;
                    }
                }
                


                if (DecodeHeaderNames){
                    for (int i = 0; i<nBlocks; i++){
                        free(DecodeHeaderNames[i]);
                    }
                    free(DecodeHeaderNames);
                }
                DecodeHeaderNames = NULL;
                decoderSetupStatus = 1;
                break;
            case 4: // BYTE GROUP SIZE
                printf("GROUPING BYTES THAT WONT FIT THE LINE MAY CAUSE TABLE DISTORTIONS.\n");
                printf("Change how many bytes per lines are displayed accordingly to fix this.\n\n");
                printf("Type how many bytes should be grouped together: ");
                scanf("%d", &grouping);

                printf("\nFeature test, enter custom grouping: ");
                free(groupingArray);
                groupingArray = malloc(bytesPerLine*sizeof(int));
                scanf(" %[^\n]s", &groupingStr);
                lastpos = splitnums(groupingStr, groupingArray);
                groupingByteSum = 0;
                for (int i=0;i<lastpos;i++){
                    groupingByteSum += groupingArray[i];
                }

                if (bytesPerLine > groupingByteSum){
                    // Makes things like: [2, 1, 1, TRASH, TRASH, TRASH] with lastpos = 3
                    // become this: [2, 1, 1, 2, 1, 1] with lastpos = 6
                    int posStep = lastpos;
                    int maxIterations = (bytesPerLine / groupingByteSum) - 1;
                    int remainingBytes = bytesPerLine % groupingByteSum;

                    // Repeats grouping pattern.
                    for (int i=0; i < maxIterations; i++){
                        for (int j = lastpos; j < lastpos + posStep; j++){
                            groupingArray[j] = groupingArray[j % posStep];
                            groupingByteSum += groupingArray[j % posStep];
                        }
                        lastpos += posStep;
                    }

                    // Fills the rest of the bytes in the line.
                    for (int i=0; i < remainingBytes ; i++){
                        groupingArray[lastpos + i] = 1;
                        groupingByteSum++;
                    }
                    lastpos += remainingBytes;

                }
                printf("\nlast pos: %d", lastpos);
                printf("\nGroupByteSum: %d", groupingByteSum);
                printf("\nArray: ");
                for (int i=0;i<lastpos;i++){
                    printf(" %d", groupingArray[i]);
                }
                scanf(" %[^\n]s", &groupingStr);
                break;
            case 5: // DEC/HEX OFFSET SWITCH
                offsetHex = (offsetHex) ? 0 : 1;
                break;
            case 6: // SET HEADER TITLES.
                if (nBlocks > 16){
                    printf("Number of decoded blocks exceeds the maximum amount of headers supported (16).\n");
                    break;
                } 
                printf("The number of columns is set as the same as the number of decodable blocks.\n\n");
                printf("It's HIGHLY RECOMMENDED to setup byte grouping, decoding and number of bytes\n");
                printf("per line before adding headers to the decoded info.\n");

                char backOption[20];
                if (decoderSetupStatus == 0){
                    printf("\nType \"continue\" if you wish to proceed, type \"back\" to go back: ");

                    scanf(" %s", backOption);
                    if (strcmp(backOption, "back")==0) break;
                }
                
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
