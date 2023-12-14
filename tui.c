#include "tui.h"
#include <stdlib.h>

void limpaTela(void){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void horizontalFrame(int n, char chStart, char chEnd){
    if (chStart) printf("%c", chStart);
    for(int i =0; i<n; i++){
        printf("\xCD");
    }
    if (chEnd) printf("%c", chEnd);
}

void horizontalFrameln(int n, char chStart, char chEnd){
    horizontalFrame(n, chStart, chEnd);
    printf("\n");
}



void hexdump(FILE *arq, int offset, char *name, int *dataTypes, int nBlocks){
    int readStatus = 1, line = 0;
    unsigned char byte;
    int currentBlock = 0, bytesParsed = 0, nextByte = 0, dataTypeWalk=0;
    int auxInt;
    char auxChar;
    float auxFloat;

    fseek(arq, 0, SEEK_SET);

    // Name tab.
    horizontalFrameln(strlen(name) + 2, 201, 187);              // ╔  ═   ╗
    printf("\xBA %s \xBA\n", name);                             // ║ name ║

    //Header ╠  ═  ╩  ═  ╗
    horizontalFrame(strlen(name) + 2, 204, 202);                // ╠  ═   ╩
    horizontalFrameln((12 + 8*offset)-strlen(name), 0, 187);    //            ═ ╗

    // ║ Offset    ║   00      01  .... ║
    printf("\xBA Offset    \xBA");
    for(int i=0; i<offset;i++){
        printf("\t%02X",i);
    }
    printf("\t\xBA\n");

    // ╠  ═  ╬  ═  ═╣
    horizontalFrame(11, 204, 206);
    horizontalFrameln(3 + (8*offset), 0, 185);
    
    //Actual table
    while(readStatus){
        printf("\xBA   %02X\t    \xBA   ", line);
        for (int byteNumber = line; byteNumber < (line+offset); byteNumber++){
            readStatus = fread(&byte, sizeof(unsigned char), 1, arq);

            if (readStatus){
                printf("0x%02X\t", byte);
            } 
            else{
                printf("\t");
            }
        }
        printf("\xBA"); //  ║ 

        int parseStart = ftell(arq) - offset;
        while(parseStart < nextByte) parseStart++;
        int parseEnd = ftell(arq);      

        fseek(arq, parseStart, SEEK_SET);
        // printf("start: %d\n", parseStart);
        // printf("end: %d\n", parseEnd);

        while(nextByte < parseEnd){
            switch (dataTypes[dataTypeWalk%nBlocks]){
                //char
                case 0:
                    nextByte += 1;
                    fread(&auxChar, sizeof(char), 1, arq);
                    if ((0 <= auxChar) && (auxChar <= 37)) auxChar = 46;
                    printf("%c ", auxChar);
                    break;
                //int
                case 1:
                    nextByte += 4;
                    fread(&auxInt, sizeof(int), 1, arq);
                    printf("%d ", auxInt);
                    break;
                //float
                case 2:
                    nextByte += 4;
                    fread(&auxFloat, sizeof(float), 1, arq);
                    printf("%f ", auxFloat);
                    break;
                //should never land here, but it's good practice to have a default case for maintainability.
                default:
                    nextByte += 1;
                    fread(&auxChar, sizeof(char), 1, arq);
                    printf("%c", auxChar);
                    break;
            }
            dataTypeWalk++;
        }
        fseek(arq, parseEnd, SEEK_SET);


        printf("\n"); 

        readStatus = fread(&byte, sizeof(unsigned char), 1, arq);
        fseek(arq, -1, SEEK_CUR);
        line += offset;
    }

    //Bottom bar.
    // ╚  ═  ╩  ═  ╝
    horizontalFrame(11, 200, 202);
    horizontalFrameln(3 + (8*offset), 0, 188);

}
