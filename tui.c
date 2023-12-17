#include "tui.h"
#include <stdlib.h>

void clearScreen(void){
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



void hexdump(FILE *arq, int offset, char *name, int *dataTypes, int nBlocks, int grouping, int offsetHex, char **DecodeHeaderNames){   
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

    // Line 1 - Border of Name tab.
    horizontalFrameln(strlen(name) + 2, 201, 187);              // ╔  ═   ╗
    // Line 2 - Body of Name tab. 
    printf("\xBA %s \xBA\n", name);                             // ║ name ║

    // Line 3 - Header ╠  ═  ╩  ═  ╗
    horizontalFrame(strlen(name) + 2, 204, 202);                // ╠  ═   ╩
    horizontalFrameln((12 + magicspace)-strlen(name), 0, 187);    //            ═ ╗


    // Line 4 -  ║ Offset    ║   00      01  ...
    printf("\xBA Offset    \xBA");
    for(int i=0; i<offset;i++){
        if ((i%grouping) == 0){
            if (offsetHex==0){
                printf("\t%02X",i);
            }
            else{
                printf("\t%02d",i);
            }
            
        }
        else{
            printf("  ");
        }
    }
    // ... ║ ...
    printf("\t\xBA"); 

    // ... header_1 ║ header_2 ║ header_3 ║
    if (DecodeHeaderNames){
        for(int i=0; i<nBlocks;i++){
            int namelenght = strlen(DecodeHeaderNames[i]);
            if ((namelenght < 5) && dataTypes[i] != 0){
                int missing = 5 - namelenght;
                for (int j = 0; j<missing/2; j++){
                    printf(" ");
                }
                printf(" %s ", DecodeHeaderNames[i]);
                for (int j = 0; j<(missing/2); j++){
                    printf(" ");
                }
                if (missing%2 != 0) printf(" ");
                printf("\xBA");
            }
            else{
                printf(" %s \xBA", DecodeHeaderNames[i]);
            }

        }
    }
    printf("\n");


    // Line 5 - ╠  ═  ╬  ═  ═╣
    horizontalFrame(11, 204, 206);
    horizontalFrameln(3 + magicspace, 0, 185);
    
    
    //Actual table
    while(readStatus){
        // line index.
        if (offsetHex==0){
            printf("\xBA   %02X\t    \xBA   ", line);
        }
        else{
            printf("\xBA   %02d\t    \xBA   ", line);
        }
        for (int byteNumber = line; byteNumber < (line+offset); byteNumber++){
            readStatus = fread(&byte, sizeof(unsigned char), 1, arq);

            if (readStatus){
                // byte printing.
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
        
        int decodeHeaderPos = 0;

        //Makes the decoding read reasonably within the line.
        while(nextByte < parseEnd){

            switch (dataTypes[dataTypeWalk%nBlocks]){
                //char
                case 0:
                    nextByte += 1;
                    fread(&auxChar, sizeof(char), 1, arq);
                    if ((0 <= auxChar) && (auxChar <= 37)) auxChar = 46;
                    if (DecodeHeaderNames){ // with header
                        int headerwidth = strlen(DecodeHeaderNames[decodeHeaderPos]);

                        int spaceLeft = headerwidth/2;
                        int spaceRight = spaceLeft;
                        if (headerwidth % 2 == 0) spaceRight = (headerwidth/2 - 1);

                        for (int j = 0; j<spaceLeft; j++){
                            printf(" ");
                        }
                        printf(" %c ", auxChar);

                        for (int j = 0; j<spaceRight; j++){
                            printf(" ");
                        }
                        printf("\xBA");
                    }
                    else{ // without header
                        printf("%c ", auxChar);
                    }
                    break;
                //int
                case 1:
                    nextByte += 4;
                    fread(&auxInt, sizeof(int), 1, arq);
                    if (DecodeHeaderNames){ // with header
                        int headerwidth = strlen(DecodeHeaderNames[decodeHeaderPos]);
                        if (headerwidth < 5) headerwidth = 5;
                        
                        headerwidth -= 5;
                        int spaceLeft = headerwidth/2;
                        int spaceRight = spaceLeft;
                        if (headerwidth % 2 != 0) spaceRight = (headerwidth/2) + 1;
                        
                        for (int j = 0; j<spaceLeft; j++){
                            printf(" ");
                        }
                        printf(" %5d ", auxInt);

                        for (int j = 0; j<spaceRight; j++){
                            printf(" ");
                        }
                        printf("\xBA");

                    }
                    else{ // without header
                        printf("%d ", auxInt);
                    }
                    break;
                //float
                case 2:
                    nextByte += 4;
                    fread(&auxFloat, sizeof(float), 1, arq);
                    if (DecodeHeaderNames){ // with header
                        int headerwidth = strlen(DecodeHeaderNames[decodeHeaderPos]);
                        if (headerwidth < 5) headerwidth = 5;
                        
                        headerwidth -= 5;
                        int spaceLeft = headerwidth/2;
                        int spaceRight = spaceLeft;
                        if (headerwidth % 2 != 0) spaceRight = (headerwidth/2) + 1;
                        
                        for (int j = 0; j<spaceLeft; j++){
                            printf(" ");
                        }
                        printf(" %5.2f ", auxFloat);

                        for (int j = 0; j<spaceRight; j++){
                            printf(" ");
                        }
                        printf("\xBA");
                    }
                    else{
                        printf("%f ", auxFloat);
                    }                    
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
            decodeHeaderPos = (decodeHeaderPos + 1) % nBlocks;
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
    // Last line -  ╚  ═  ╩  ═  ╝
    horizontalFrame(11, 200, 202);
    horizontalFrameln(3 + magicspace, 0, 188);

}


const char* getSuffix(int num) {
    if (num >= 10 && num <= 20) {
        return "th";
    } else {
        switch (num % 10) {
            case 1: return "st";
            case 2: return "nd";
            case 3: return "rd";
            default: return "th";
        }
    }
}