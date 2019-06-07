#ifndef PROCESSESOPTION2_H_
#define PROCESSESOPTION2_H_

#include "Utilities.h"
#include <unistd.h>

typedef struct {
	int board[81];
	bool boardReady;
	bool result[3];
	bool resultReady[3];
	bool noMoreBoards;
} sharedMemorySudoku;

int processesCreator(pid_t *pid, howToCheck *byRowsColsOrSquares);
int masterProcess(int argc, char **argv, sharedMemorySudoku *sharedSudoku);
void childProccessJob(sharedMemorySudoku *sharedSudoku,
		howToCheck RowsOrColsOrMatrix);

#endif /* PROCESSESOPTION2_H_ */
