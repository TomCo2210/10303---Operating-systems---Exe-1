
#ifndef PROCESSESOPTION1_H_
#define PROCESSESOPTION1_H_

void processesCreator(int **pFdRead, int **pFdWrite1, int **pFdWrite2, int **pFdWrite3,
		char* argv0);
void masterProcess(int argc, char **argv, int *fdRead, int *fdWrite1, int *fdWrite2, int *fdWrite3);

#endif /* PROCESSESOPTION1_H_ */
