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



void hexdump(FILE *arq, int offset, char *name, int *dataTypes, int nBlocks, int grouping){
    int readStatus = 1, line = 0;
    unsigned char byte;
    int currentBlock = 0, bytesParsed = 0, nextByte = 0, dataTypeWalk=0;
    int auxInt;
    char auxChar;
    float auxFloat;
    // int grouping = 1;
    int spacing = ((grouping*2)/4) + 1;

    //2*grouping representa quantos caracteres o print dos bytes agrupados ocupam.
    //+ 8 representa o valor de um "tab" em espaços.
    // -2*grouping%8 é o quanto o espaçamento de 8 diminui de acordo com que o print
    //_dos bytes ocupa mais espaço.
    // * (offset/grouping) é o número de grupos em um espaço de offset.
    int magicspace = (2*grouping + 8 - 2*grouping%8)*(offset/grouping);
    fseek(arq, 0, SEEK_SET);

    // Name tab.
    horizontalFrameln(strlen(name) + 2, 201, 187);              // ╔  ═   ╗
    printf("\xBA %s \xBA\n", name);                             // ║ name ║

    //Header ╠  ═  ╩  ═  ╗
    horizontalFrame(strlen(name) + 2, 204, 202);                // ╠  ═   ╩
    horizontalFrameln((12 + magicspace)-strlen(name), 0, 187);    //            ═ ╗


    // ║ Offset    ║   00      01  .... ║
    printf("\xBA Offset    \xBA");
    for(int i=0; i<offset;i++){
        if ((i%grouping) == 0){
            printf("\t%02X",i);
        }
        else{
            printf("  ");
        }
    }
    printf("\t\xBA\n");

    // ╠  ═  ╬  ═  ═╣
    horizontalFrame(11, 204, 206);
    // horizontalFrameln(3 + (8*offset), 0, 185);
    horizontalFrameln(3 + magicspace, 0, 185);
    
    
    //Actual table
    while(readStatus){
        printf("\xBA   %02X\t    \xBA   ", line);
        for (int byteNumber = line; byteNumber < (line+offset); byteNumber++){
            readStatus = fread(&byte, sizeof(unsigned char), 1, arq);
            
            if (readStatus){
                if (grouping == 1){
                    printf("0x%02X", byte);
                }
                else{
                    printf("%02X", byte);
                }

                // tab between groupings.
                if ((byteNumber+1) % grouping == 0) printf("\t");
            } 
            else{ //empty spaces for non existing bytes, either "ghost" 0xXX or XX.
                if (grouping == 1){
                    printf("    ");
                }
                else{
                    printf("  ");
                }
                //aplica tab para cada grupo bytes "completo".
                if ((byteNumber+1) % grouping == 0) printf("\t");
            }
        }
        printf("\xBA"); //  ║ 

        //Decoding section.
        int parseStart = ftell(arq) - offset;
        while(parseStart < nextByte) parseStart++;
        int parseEnd = ftell(arq);      

        fseek(arq, parseStart, SEEK_SET);
        
        //Makes the decoding read reasonably within the line.
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
                    if ((0 <= auxChar) && (auxChar <= 37)) auxChar = 46;
                    printf("%c ", auxChar);
                    break;
            }
            dataTypeWalk++;
        }
        //Resets cursor right after the end of the line in case the last read exceeded the line end.
        fseek(arq, parseEnd, SEEK_SET);

        printf("\n"); 

        //Checks next byte from next line to see if it's empty.
        readStatus = fread(&byte, sizeof(unsigned char), 1, arq);
        fseek(arq, -1, SEEK_CUR);
        line += offset;
    }

    //Bottom bar.
    // ╚  ═  ╩  ═  ╝
    horizontalFrame(11, 200, 202);
    horizontalFrameln(3 + magicspace, 0, 188);

}
