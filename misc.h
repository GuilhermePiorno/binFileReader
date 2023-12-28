// Transforms a string of spaced numbers into an array of ints.
// Returns the array size.
int SplitNums(char* str, int* array); 

// Counts how many bytes are in a custom grouping array.
int CountGroupingSize(int* array, int endPos);

// Fits grouping into a line by repetition or filling with 1's
// returns size of the array.
int MatchGroupingToLine(int* groupingArray, int arraySize, int bytesPerLine);

// Accumulates values of the gruping array.
// Example:
// from: [2, 3, 2, 2, 3, 2, 1 ,1]
// to:   [2, 5, 7, 9, 12, 14, 15, 16]
void CustomGroupingAccumulator(int* array, int arraysize);

