

#include "Utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

void readBoard(char* boardAsString, int fd) {
	int i, bytesRead = 0;
	char lineAsString[31];

	strcpy(boardAsString, "");
	if (!fd)
		printf("enter a sudoku board row by row\n");

	for (i = 0; i < 9; i++) {
		if (!fd) {
			printf("enter %d row:\n", i + 1);
			bytesRead = read(STDIN_FILENO, lineAsString, 30);
			if (bytesRead == -1) {
				fprintf(stderr,
						"ERROR in %s() function from \"%s\" file, line %d: %s\n",
						__FUNCTION__, __FILE__, __LINE__, strerror(errno));
				exit(EXIT_FAILURE);
			}
			lineAsString[bytesRead] = '\0';
		} else {
			bytesRead = read(fd, lineAsString, 30);
			if (bytesRead == -1) {
				fprintf(stderr,
						"ERROR in %s() function from \"%s\" file, line %d: %s\n",
						__FUNCTION__, __FILE__, __LINE__, strerror(errno));
				exit(EXIT_FAILURE);
			}
			lineAsString[bytesRead] = '\0';
		}
		strcat(boardAsString, lineAsString);
	}
}

void boardAtoI(char* boardAsString, int stringSize,
		int* board) {
	int i, counter = 0;
	for (i = 0; i < 81; i++) {
		while (!isdigit(boardAsString[counter]) && counter < stringSize)
			counter++;
		if (counter == stringSize) {
			fprintf(stderr,
					"ERROR in %s() function from \"%s\" file, line %d: %s\n",
					__FUNCTION__, __FILE__, __LINE__, strerror(errno));
			exit(EXIT_FAILURE);
		}
		*(board + i) = boardAsString[counter++] - '0';
	}
}

void printBoard(int* board) {
	int i;
	for (i = 0; i < SUDOKU_BOARD_SIZE; i++) {
		printf("%d ", *(board + i));
		if ((i + 1) % 9 == 0)
			printf("\n");
	}
}

