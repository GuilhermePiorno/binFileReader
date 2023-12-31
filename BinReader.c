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
#define BPL_DEFAULT_VALUE 16




int main(void){
    clearScreen();
    int bytesPerLine = BPL_DEFAULT_VALUE; // (Bytes per row. Offset)
    int op, continua = 1;
    int nBlocks=1;
    int *dataTypes = malloc(sizeof(int));
    int *groupingArray = malloc(bytesPerLine*sizeof(int));
    dataTypes[0] = CHAR;
    int offsetHex = 0; // 0 for hex, 1 for dec.
    char **DecodeHeaderNames = NULL;
    int decoderSetupStatus = 0;
    int bytesPerLinePrediction;
    int bytesPerDataType;
    char groupingStr[100]; // Actual max size should be 2*bytesperline to cover the case "1 1 ... 1"
    int lastPos, groupingByteSum;
    char reply;

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

    bytesPerLine = BPL_DEFAULT_VALUE;
    //Default value for grouping.
    for (int i=0; i<bytesPerLine; i++){
        groupingArray[i] = i + 1; 
    }
    lastPos = BPL_DEFAULT_VALUE;
    
    while(continua){
        clearScreen();
        hexdump(arquivo, bytesPerLine, nomeArquivo, dataTypes, nBlocks, offsetHex, DecodeHeaderNames, groupingArray, lastPos);
        printf("[1] Open file...\n");
        printf("[2] Adjust bytes per line displayed.\n");
        printf("[3] Decoder configuration.\n");
        printf("[4] Change byte group size.\n");
        printf("[5] Toggle offset display between Hex & Dec.\n");
        printf("[6] Set Decoder header.\n");
        printf("[9] Exit\n");
        scanf("%d", &op);
        clearScreen();
        hexdump(arquivo, bytesPerLine, nomeArquivo, dataTypes, nBlocks, offsetHex, DecodeHeaderNames, groupingArray, lastPos);

        switch(op){
            case 1: // OPEN FILE.
                //Clearing some values to avoid memleak.
                free(groupingArray);
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

                // Reseting values.
                bytesPerLine = BPL_DEFAULT_VALUE;
                groupingArray = malloc(bytesPerLine*sizeof(int));
                for (int i=0; i<bytesPerLine; i++){
                    groupingArray[i] = i + 1; 
                }
                lastPos = BPL_DEFAULT_VALUE;
                decoderSetupStatus = 0;
                break;
            case 2: // BYTES PER LINE.
                printf("CHANGING THIS CONFIGURATION WILL RESET THE BYTE GROUPING SETUP\n\n");
                printf("Number of bytes per line: ");
                scanf("%d", &bytesPerLine);

                free(groupingArray);
                groupingArray = malloc(bytesPerLine*sizeof(int));
                for (int i=0; i<bytesPerLine; i++){
                    groupingArray[i] = i + 1; 
                }
                lastPos = bytesPerLine;

                break;
            case 3: // DECODER SETUP
                // If nblocks is reset before a free, the extent of DecoderHeaderNames would be lost.
                bytesPerLinePrediction = 0;
                if (DecodeHeaderNames){
                    for (int i = 0; i<nBlocks; i++){
                        free(DecodeHeaderNames[i]);
                    }
                    free(DecodeHeaderNames);
                }
                DecodeHeaderNames = NULL;
                decoderSetupStatus = 1;

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

                
                if (bytesPerLinePrediction != bytesPerLine){
                    printf("Do you want to auto-adjust the number of bytes per line accordingly? (y/n): ");
                    scanf(" %c", &reply);
                    reply = tolower(reply);
                    printf("%c", reply);
                    if (reply == 'y'){
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
                printf("You can enter an integer for simple grouping of that number of bytes\n");
                printf("You can also enter a pattern to create a custom grouping.\n");
                printf("Example: 4 2 2 (groups bytes in a pattern of 4 2 2 until it reaches the end of the line)\n\n");
                printf("Type how many bytes should be grouped together: ");


                free(groupingArray);
                groupingArray = malloc(bytesPerLine*sizeof(int));
                scanf(" %[^\n]s", groupingStr);

                lastPos = SplitNums(groupingStr, groupingArray);
                groupingByteSum = CountGroupingSize(groupingArray, lastPos);


                if (bytesPerLine >= groupingByteSum){
                    lastPos = MatchGroupingToLine(groupingArray, lastPos, bytesPerLine);
                }else{
                    printf("The grouping requested doesn't currently fit the current amount of bytes per line.\n");
                    printf("Do you wish to auto-adjust the lenght of the line? (y/n)\n");
                    printf("If not, custom grouping will be rejected. ");
                    scanf(" %c", &reply);
                    if (reply == 'y'){
                        bytesPerLine = groupingByteSum;
                    }
                    else{
                        groupingArray[0] = 1;
                        lastPos = MatchGroupingToLine(groupingArray, 1, bytesPerLine);
                    }
                }
                groupingByteSum = CountGroupingSize(groupingArray, lastPos);

                CustomGroupingAccumulator(groupingArray, lastPos);

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

                // Gives the user a way out. (could be treated with only a char y/n.. maybe change that.)
                char backOption[20];
                if (decoderSetupStatus == 0){
                    printf("\nType \"continue\" if you wish to proceed, type \"back\" to go back: ");

                    scanf(" %s", backOption);
                    if (strcmp(backOption, "back")==0) break;
                }
                
                // Frees before setting up a variable that could be allocated.
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
    free(groupingArray);
    free(dataTypes);
    fclose(arquivo);

    return 0;
}
