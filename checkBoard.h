#ifndef CHECKSUDOKU_H_
#define CHECKSUDOKU_H_

#include "Utilities.h"

bool checkBoard(howToCheck byRowsColsOrSquares,
		int* board);
int checkRows(int* arr, int row) ;
int checkColumns(int* arr, int col);
int checkSquare(int* arr, int box);

#endif /* CHECKSUDOKU_H_ */
