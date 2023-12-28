#include <stdio.h>
#include <string.h>

// Limpa a tela.
void clearScreen(void);

// Imprime n separadores horizontais  com inicio e fim.
void horizontalFrame(int n, char chStart, char chEnd);

// Imprime n separadores horizontais com inicio e fim.\n no fim.
void horizontalFrameln(int n, char chStart, char chEnd);


// Hexdump with decoding.
void hexdump(FILE *arq, int offset, char *name, int *dataTypes, int nBlocks, int grouping, int offsetHex, char** DecodeHeaderNames, int* customGroupingArray, int customGroupingSize);

//Suffix for ordinal numbers.
const char* getSuffix(int num);


