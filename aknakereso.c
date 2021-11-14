#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const int size = 10;
const int maxBombsNum = 30;
const int minBombsNum = 3;

// used in the printTable function for X character
const int valueForBomb = 11;
const int emptyValueForPlayer = -1;
const int initNumOfBombs = -1;

int bombs[size][size];
int chooses[size][size];
int numOfBombs = initNumOfBombs;
int numOfLeft;
int boom = 0;

int rowCoord = -1;
int columnCoord = -1;

void printTable(int table[size][size]);
void initPlayerTable();
void getNumberOfBombs();
void placeBombsInTable(int numOfBombs);
void getCoords();
void writeCountOfNeighbourBombs(int rowIndex, int columnIndex);
void checkCoords();


int main(int argc, char* argv[]) {
    initPlayerTable();
    getNumberOfBombs(argc, argv);
    placeBombsInTable(numOfBombs);
    printTable(chooses);

    while (numOfLeft > 0 && boom != 1) {
        getCoords();
        checkCoords();
        printTable(chooses);
    }

    if (boom == 1) {
        printf("\nThat was a bomb. You failed this time. Dare you start again?\n");
    } else {
        printf("\nNice job, found/avoided all bombs!\n");
    }

    return 0;
}

void printTable(int table[size][size]) {
    // print first row 0-9
    printf(" ");
    for (int i=0; i<=9; i++) {
        printf(" %d", i);
    }
    printf("\n");

    // print rows starting with A-J
    char chars[10] = "ABCDEFGHIJ";
    for (int i=0; i<10; i++) {
        printf("%c ", chars[i]);

        for (int j=0; j<10; j++) {
            if (table[i][j] == -1) {
                printf("  ");
            } else if (table[i][j] == 11) {
                printf("X ");
            } else {
                printf("%d ", table[i][j]);
            }
        }

        printf("\n");
    }
}

void initPlayerTable() {
    for (int i=0; i<=9; i++) {
        for (int j=0; j<=9; j++) {
            chooses[i][j] = emptyValueForPlayer;
        }
    }
}

void getNumberOfBombs(int argc, char* argv[]) {
    // handle command line input for bomb number
    if (argc >= 2) {
        int inputNum = atoi(argv[1]);
        
        if (argv[1][0] == '0' && inputNum == 0) {
            numOfBombs = 0;
        } else if (inputNum == 0) {
            // handle invalid input below
        } else {
            numOfBombs = inputNum;
        }
        
    }

    // handle no command line input for bomb number + wrong input
    int isNumInitialised = argc >= 2 && numOfBombs == initNumOfBombs;
    int isNumInRange = numOfBombs < minBombsNum || numOfBombs > maxBombsNum;
    
    if (argc == 1 || isNumInitialised || isNumInRange) {
        printf("Give number of bombs (%d-%d): ", minBombsNum, maxBombsNum);
        scanf("%d", &numOfBombs);

        while (numOfBombs < minBombsNum || numOfBombs > maxBombsNum) {
            printf("\nChoose between %d and %d please: ", minBombsNum, maxBombsNum);
            scanf("%d", &numOfBombs);
        }
        printf("\n");
    }


    numOfLeft = 100 - numOfBombs;
}

void placeBombsInTable(int numOfBombs) {
    srand(time(0));

    int i=0;
    while (i<numOfBombs) {
        int randomRow = rand() % 10;
        int randomColumn = rand() % 10;

        if (bombs[randomRow][randomColumn] != 1) {
            bombs[randomRow][randomColumn] = 1;
            i++;
        }
    }
}

void getCoords() {
    char coords[3];

    int foundRowCoord = 0;
    int foundColumnCoord = 0;

    while (foundRowCoord == 0 || foundColumnCoord == 0) {
        printf("\nChoose coordinates (example: E4): ");
        scanf("%s", coords);
        printf("\n");

        foundRowCoord = 0;
        foundColumnCoord = 0;

        int i=0;
        while (i<10 && foundRowCoord != 1) {
            if ("ABCDEFGHIJ"[i] == coords[0]) {
                rowCoord = i;
                foundRowCoord = 1;
            }
            i++;
        }

        int j=0;
        while (j<10 && foundColumnCoord != 1) {
            if ("0123456789"[j] == coords[1]) {
                columnCoord = j;
                foundColumnCoord = 1;
            }
            j++;
        }
    }

}

void writeCountOfNeighbourBombs(int rowIndex, int columnIndex) {
    int count = 0;
    for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
            if (i==0 && j==0) {}
            else if (rowIndex+i >=0 && rowIndex+i <= 9 && columnIndex+j >= 0 && columnIndex+j <= 9) {
                if (bombs[rowIndex+i][columnIndex+j] == 1) {
                    count++;
                }
            }
        }
    }

    chooses[rowCoord][columnCoord] = count;
}

void checkCoords() {
    int* bombCell = &bombs[rowCoord][columnCoord];
    int* playerCell = &chooses[rowCoord][columnCoord];

    if (*bombCell == 1) {
        boom = 1;
        *playerCell = valueForBomb;
    } else {
        writeCountOfNeighbourBombs(rowCoord, columnCoord);
        numOfLeft--;
    }
}
