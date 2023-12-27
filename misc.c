#include "misc.h"
#include <stdlib.h>

int SplitNums(char *str, int* array) {
  char *token = strtok(str, " ");
  int count = 0;
  
  while (token != NULL) {
    array[count] = atoi(token);
    count++;
    token = strtok(NULL, " ");
  }
  return count;
}

int CountGroupingSize(int* array, int endPos){
  int groupingByteSum = 0;
  for (int i=0;i<endPos;i++){
      groupingByteSum += array[i];
  }
  return groupingByteSum;
}

int MatchGroupingToLine(int* groupingArray, int arraySize, int bytesPerLine){
  int groupingByteSum = CountGroupingSize(groupingArray, arraySize);
  int posStep = arraySize;
  int maxIterations = (bytesPerLine / groupingByteSum) - 1;
  int remainingBytes = bytesPerLine % groupingByteSum;

  // Repeats grouping pattern.
  for (int i=0; i < maxIterations; i++){
      for (int j = arraySize; j < arraySize + posStep; j++){
          groupingArray[j] = groupingArray[j % posStep];
          groupingByteSum += groupingArray[j % posStep];
      }
      arraySize += posStep;
  }

  // Fills the rest of the bytes in the line.
  for (int i=0; i < remainingBytes ; i++){
      groupingArray[arraySize + i] = 1;
      groupingByteSum++;
  }
  arraySize += remainingBytes;  

  return arraySize;
}

void CustomGroupingAccumulator(int* array, int arraysize){
  if (arraysize == 1) return;
  for (int i = 1; i < arraysize; i++){
    array[i] += array[i-1];
  }
}