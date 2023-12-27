#include "misc.h"
#include <stdlib.h>

int splitnums(char *str, int* array) {
  char *token = strtok(str, " ");
  int count = 0;
  
  while (token != NULL) {
    array[count] = atoi(token);
    count++;
    token = strtok(NULL, " ");
  }
  return count;
}