#include "frame.h"

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