#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Exe1Processes_V1.h"

#include "Utilities.h"

const char* howToCheckOptions[] = { "0", "1", "2" };
const char* FILE_PATH = "./check";

int main(int argc, char **argv) {
	int fdRead[2], fdWrite1[2], fdWrite2[2], fdWrite3[2];
	int *pfdRead = fdRead, *pfdWrite1 = fdWrite1, *pfdWrite2 = fdWrite2,
			*pfdWrite3 = fdWrite3;

	processesCreator(&pfdRead, &pfdWrite1, &pfdWrite2, &pfdWrite3, argv[0]);
	masterProcess(argc, argv, fdRead, fdWrite1, fdWrite2, fdWrite3);

	return EXIT_SUCCESS;
}

void masterProcess(int argc, char **argv, int *fdRead, int *fdWrite1,
		int *fdWrite2, int *fdWrite3) {
	bool consoleRead = FALSE, result = TRUE;
	int numberOfBoards = 0, i, j;
	int board[SUDOKU_BOARD_SIZE];
	char boardAsString[STRING_BOARD_SIZE];
	int fd = 0;
	char answer[3];
	char* nameOfFile = "matrix";

	if (argc == 1) {
		consoleRead = TRUE;
		numberOfBoards = 1;
	} else

		numberOfBoards = argc - 1;

	for (i = 1; i <= numberOfBoards; i++) {

		if (consoleRead == TRUE) {
			readBoard(boardAsString, 0);
		} else {
			nameOfFile = argv[i];
			fd = open(nameOfFile, O_RDONLY);
			if (fd == -1) {
				exit(EXIT_FAILURE);
			}

			// the input is now the file
			readBoard(boardAsString, fd);
			close(fd);
		}

		boardAtoI(boardAsString, STRING_BOARD_SIZE, board);

		write(fdWrite1[1], board, 81 * sizeof(int));
		write(fdWrite2[1], board, 81 * sizeof(int));
		write(fdWrite3[1], board, 81 * sizeof(int));

		read(fdRead[0], answer + 0, sizeof(char));
		read(fdRead[0], answer + 1, sizeof(char));
		read(fdRead[0], answer + 2, sizeof(char));

		result = TRUE;
		for (j = 0; j < 3 && result; j++) {
			if (answer[j] == '0') {
				result = FALSE;
			}
		}

		fprintf(stdout, "%s is%s legal\n", nameOfFile, result ? "" : " NOT");
	}

	close(fdWrite1[1]);
	close(fdWrite2[1]);
	close(fdWrite3[1]);
	close(fdRead[0]);

}

void processesCreator(int **pfdRead, int **pfdWrite1, int **pfdWrite2,
		int **pfdWrite3, char* argv0) {
	int i, execlpReturn = 0;
	pid_t pid;
	int ***pWriteFd = NULL;
	howToCheck byRowsColsOrSquares;

	if (pipe(*pfdRead)) {
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 3; i++) {
		switch (i) {
		case 0:
			pWriteFd = &pfdWrite1;
			break;
		case 1:
			pWriteFd = &pfdWrite2;
			break;
		case 2:
			pWriteFd = &pfdWrite3;
			break;
		}

		if (pipe(**pWriteFd)) {
			exit(EXIT_FAILURE);
		}

		pid = fork();

		if (pid < 0) {
			exit(EXIT_FAILURE);

		} else if (pid == 0) {
			close((**pWriteFd)[1]);
			close((*pfdRead)[0]);
			if ((**pWriteFd)[0] != STDIN_FILENO) {
				dup2((**pWriteFd)[0], STDIN_FILENO);
				close((**pWriteFd)[0]);
			}
			if ((*pfdRead)[1] != STDOUT_FILENO) {
				dup2((*pfdRead)[1], STDOUT_FILENO);
				close((*pfdRead)[1]);
			}
			byRowsColsOrSquares = (howToCheck) i;
			execlpReturn = execlp(FILE_PATH, argv0,
					howToCheckOptions[byRowsColsOrSquares], (char*) NULL);
			if (execlpReturn < 0) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		close((**pWriteFd)[0]);
	}
	close((*pfdRead)[1]);
}

