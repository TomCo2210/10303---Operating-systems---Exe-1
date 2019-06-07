all: v1 v2 check

v1:
	gcc -o v1 Exe1Processes_V1.c Utilities.c
v2:
	gcc -o v2 Exe1Processes_V2.c checkBoard.c Utilities.c
check:
	gcc -o check checkBoardRunnable.c Utilities.c
clean:
	rm -f v1 v2 check
