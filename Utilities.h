#ifndef UTILS_H_
#define UTILS_H_

#define STRING_BOARD_SIZE 300
#define SUDOKU_BOARD_SIZE 81
#define ROWS 9
#define COLUMNS 9
#define SQUARE 9
#define SQUARE_DIM 3

typedef enum {
	BY_ROWS, BY_COLUMNS, BY_SQUARE
} howToCheck;
typedef enum {
	FALSE, TRUE
} bool;

void readBoard(char* boardAsString, int fd);
void boardAtoI(char* boardAsString, int stringSize,
		int* board);
void printBoard(int* board);

#endif /* UTILS_H_ */
