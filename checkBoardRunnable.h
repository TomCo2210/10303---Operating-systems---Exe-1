#ifndef CHECKSUDOKUWITHMAIN_H_
#define CHECKSUDOKUWITHMAIN_H_

#include "Utilities.h"

bool checkBoard(howToCheck byRowsColsOrSquares, int* board);
int checkRows(int* arr, int row);
int checkColumns(int* arr, int col);
int checkSquare(int* arr, int box);

#endif /* CHECKSUDOKUWITHMAIN_H_ */
