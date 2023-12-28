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



void hexdump(FILE *arq, int offset, char *name, int *dataTypes, int nBlocks, int offsetHex, char **DecodeHeaderNames, int* customGroupingArray, int customGroupingSize){   
    int readStatus = 1, line = 0;
    unsigned char byte;
    // int currentBlock = 0, bytesParsed = 0; 
    int nextByte = 0, dataTypeWalk=0;
    int auxInt;
    char auxChar;
    float auxFloat;
    int customGroupingIndex;
    int lastprint;

    
    // 2*offset -> represents the space each byte occupies
    // Calculated Tabbed Spaces returns the actual space tabs will occupy.
    int magicspace = 2*offset + CalculateTabbedSpaces(customGroupingArray, customGroupingSize);

    fseek(arq, 0, SEEK_SET);

    // Line 1 - Border of Name tab.
    horizontalFrameln(strlen(name) + 2, 201, 187);              // ╔  ═   ╗
    // Line 2 - Body of Name tab. 
    printf("\xBA %s \xBA\n", name);                             // ║ name ║

    // Line 3 - Header ╠  ═  ╩  ═  ╗
    horizontalFrame(strlen(name) + 2, 204, 202);                // ╠  ═   ╩
    horizontalFrameln((12 + magicspace)-strlen(name), 0, 187);    //            ═ ╗


    // Line 4 -  ║ Offset    ║   00      01  ...
    printf("\xBA Offset    \xBA");  //  ║ Offset    ║ 

    if (offsetHex==0){   // 00
        printf("\t%02X", 0);
    }
    else{
        printf("\t%02d", 0);
    } 

    lastprint = 0;
    for(int j = 0; j < customGroupingArray[0] - lastprint - 1; j++){
        printf("  ");
    }



    for (int i = 0; i < customGroupingSize-1; i++){  // 01  02  03  04  ...
        if (customGroupingArray[i] >= offset) continue;
        if (offsetHex==0){
            printf("\t%02X", customGroupingArray[i]);
        }
        else{
            printf("\t%02d", customGroupingArray[i]);
        }

        lastprint = customGroupingArray[i]; 
        for(int j = 0; j < customGroupingArray[i+1] - lastprint - 1; j++){
            printf("  ");
        }
        
    }

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
        customGroupingIndex = 0;
        for (int byteNumber = line; byteNumber < (line+offset); byteNumber++){
            readStatus = fread(&byte, sizeof(unsigned char), 1, arq);
            

            if (readStatus){
                // byte printing.
                printf("%02X", byte);
            }
            else{ //empty spaces for non existing bytes, either "ghost" 0xXX or XX.
                printf("  ");
            }
            
            if (((byteNumber+1) % offset) >= customGroupingArray[customGroupingIndex]){
                printf("\t");
                customGroupingIndex += 1;
            }
        }
        printf("\t\xBA"); //  ║ 

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

int CalculateTabbedSpaces(int* array, int arraysize){
    int spaces = 0;

    //Goes back before accumulation.
    for (int i = arraysize - 1; i > 0; i--){
        array[i] -= array[i-1];
    }
    
    //Calculates the space each tab will contribute to the sum.
    for (int i = 0; i < arraysize; i++){
        spaces += 8 - (2*array[i])%8;
    }

    //Turns the array back to its original form.
    for (int i = 1; i < arraysize; i++){
        array[i] += array[i-1];
    }
    
    return spaces;
}