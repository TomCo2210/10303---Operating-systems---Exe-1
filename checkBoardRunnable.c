#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "checkBoardRunnable.h"

const char howToCheckOptions[] = { '0', '1' };

int main(int argc, char **argv) {
	howToCheck byRowsColsOrSquares;
	int board[SUDOKU_BOARD_SIZE];
	int readReturnValue = 1;
	char answer = '0';

	if (argc == 2) {
		while (readReturnValue != 0) {
			byRowsColsOrSquares = (howToCheck) (*(argv[1]) - '0');
			readReturnValue = read(STDIN_FILENO, board,
			SUDOKU_BOARD_SIZE * sizeof(int));
			if (readReturnValue != 0) {

				answer = howToCheckOptions[checkBoard(byRowsColsOrSquares,
						board)];
				write(STDOUT_FILENO, &answer, sizeof(char));
			}
		}
	} else {
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

bool checkBoard(howToCheck byRowsColsOrSquares, int* board) {
	int i, j;
	unsigned int answer = 0;

	for (i = 0; i < 9; i++) {
		if (byRowsColsOrSquares == BY_ROWS)
			answer = checkRows(board, i);
		else if (byRowsColsOrSquares == BY_COLUMNS)
			answer = checkColumns(board, i);
		else {
			answer = checkSquare(board, i);
		}
		if (!answer)
			return FALSE;
	}
	return TRUE;
}

int checkRows(int* arr, int row) {
	int numbers[ROWS] = { 0 };
	int i, neededIndex = 0;
	for (i = 0; i < ROWS; i++) {
		neededIndex = i + row * ROWS;
		numbers[arr[neededIndex] - 1]++;
		if (numbers[arr[neededIndex] - 1] > 1)
			return FALSE;
	}

	return TRUE;
}
int checkColumns(int* arr, int col) {
	int numbers[COLUMNS] = { 0 };
	int i, neededIndex = 0;
	for (i = 0; i < COLUMNS; i++) {
		neededIndex = col + i * COLUMNS;
		numbers[arr[neededIndex] - 1]++;
		if (numbers[arr[neededIndex] - 1] > 1)
			return FALSE;
	}
	return TRUE;
}

int checkSquare(int* arr, int box) {
	int numbers[SQUARE] = { 0 };
	int rows, cols, neededIndex = 0;
	for (rows = 0; rows < SQUARE_DIM; ++rows) {
		for (cols = 0; cols < SQUARE_DIM; ++cols) {
			neededIndex = ((cols / 3) + (rows / 3) * 3) * SQUARE + (cols % 3)
					+ (rows % 3) * SQUARE_DIM;
			numbers[arr[neededIndex] - 1]++;
			if (numbers[arr[neededIndex] - 1] > 1) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

