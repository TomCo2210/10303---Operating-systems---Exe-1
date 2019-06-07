#include "Exe1Processes_V2.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "checkBoard.h"
#include "Utilities.h"

int main(int argc, char **argv) {
	pid_t pid = 0;
	howToCheck byRowsColsOrSquares = 0;

	sharedMemorySudoku* sharedSudoku = mmap(NULL, sizeof(int),
	PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	if (sharedSudoku == MAP_FAILED) {
		exit(EXIT_FAILURE);
	}

	memset(sharedSudoku, 0, sizeof(sharedMemorySudoku));

	if (processesCreator(&pid, &byRowsColsOrSquares) == EXIT_FAILURE) {
		sharedSudoku->noMoreBoards = TRUE;
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		if (masterProcess(argc, argv, sharedSudoku)) {
			sharedSudoku->noMoreBoards = TRUE;
			exit(EXIT_FAILURE);
		}
	} else
		childProccessJob(sharedSudoku, byRowsColsOrSquares);
	return EXIT_SUCCESS;

}

int processesCreator(pid_t *pid, howToCheck *byRowsColsOrSquares) {
	int i;

	for (i = 0; i < 3 ; i++) {
		*pid = fork();

		if (*pid < 0) {
			return (EXIT_FAILURE);
		} else if (*pid == 0) {
			*byRowsColsOrSquares = (howToCheck) i;
			break;
		}
	}
	return EXIT_SUCCESS;
}

int masterProcess(int argc, char **argv, sharedMemorySudoku *sharedSudoku) {
	bool readFromInput = FALSE, result = TRUE, finished = FALSE;
	int numberOfBoards = 0, i, j;
	char boardAsString[STRING_BOARD_SIZE];
	int file = 0;
	char* nameOfFile = "matrix";

	if (argc == 1) {
		readFromInput = TRUE;
		numberOfBoards = 1;
	} else
		numberOfBoards = argc - 1;

	for (i = 1; i <= numberOfBoards; i++) {
		if (readFromInput == TRUE) {
			readBoard(boardAsString, 0);
		} else {
			nameOfFile = argv[i];
			file = open(nameOfFile, O_RDONLY);
			if (file == -1) {
				exit(EXIT_FAILURE);
			}

			readBoard(boardAsString, file);
			close(file);
		}

		boardAtoI(boardAsString, STRING_BOARD_SIZE, sharedSudoku->board);
		sharedSudoku->boardReady = TRUE;
		while (!finished) {
			finished = TRUE;
			for (j = 0; j < 3; j++) {
				if (sharedSudoku->resultReady[j] == FALSE) {
					finished = FALSE;
					break;
				}
			}
		}

		finished = FALSE;
		result = TRUE;
		sharedSudoku->boardReady = FALSE;
		for (j = 0; j < 3; j++) {
			sharedSudoku->resultReady[j] = FALSE;
			if (sharedSudoku->result[j] == FALSE) {
				result = FALSE;
			}
		}

		fprintf(stdout, "%s is%s legal\n", nameOfFile, result ? "" : " NOT");
	}

	sharedSudoku->noMoreBoards = TRUE;
	return EXIT_SUCCESS;
}

void childProccessJob(sharedMemorySudoku *sharedSudoku,
		howToCheck byRowsColsOrSquares) {
	bool result = FALSE;
	while (sharedSudoku->noMoreBoards == FALSE) {
		if (!(sharedSudoku->resultReady[byRowsColsOrSquares])
				&& sharedSudoku->boardReady) {
			result = checkBoard(byRowsColsOrSquares, sharedSudoku->board);
			sharedSudoku->result[byRowsColsOrSquares] = result;
			sharedSudoku->resultReady[byRowsColsOrSquares] = TRUE;
		}
	}
	exit(EXIT_SUCCESS);
}

