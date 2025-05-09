#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SEPARATOR printf("=========================================\n")
#define FSEPARATOR fprintf(fr, "=========================================\n")
#define CLEAR system("cls")
#define FNEWLINE fprintf(fr, "\n")
#define NEWLINE printf("\n")
#define MAX_FILENAME 20
#define SUDOKU_SIZE 9
#define ERROR_ARR_SIZE 6

const char* ERROR_CODE[ERROR_ARR_SIZE] = {
    "Blogi duomenys duotame faile.\n",
    "Negalima sukurti failo arba ivesties ir isvesties failu pavadinimai vienodi.\n",
    "Indeksas negali buti neigiamas.\n",
    "Indeksas negali buti didesnis uz ERROR_CODE masyvo dydi.\n",
    "Neimanoma atidaryti nurodyto failo arba jis neegzistuoja.\n",
    "Pateikta nekorektiska sudoku lenta.\n"
    };

int deepCount = -1, stepCount = 0, backtrackCount = 0;
char FDATA_NAME[MAX_FILENAME];
char FRESULT_NAME[MAX_FILENAME];
FILE* fd;
FILE* fr;

bool fetchSudoku(int[][SUDOKU_SIZE]);
bool checkData(int [][SUDOKU_SIZE]);
void printSudoku(int [][SUDOKU_SIZE]);
void fprintSudoku(int[][SUDOKU_SIZE]);
bool solveSudoku(int [][SUDOKU_SIZE]);
bool findEmpty(int [][SUDOKU_SIZE], int*, int*);
int findEmptyCount(int [][SUDOKU_SIZE]);
bool checkRow(int[][SUDOKU_SIZE], int, int);
bool checkCol(int[][SUDOKU_SIZE], int, int);
bool checkBlock(int[][SUDOKU_SIZE], int, int, int);
bool isValid(int[][SUDOKU_SIZE], int, int, int);
void printError(int errorCode);

int main()
{
    int sudokuBoard[SUDOKU_SIZE][SUDOKU_SIZE];
    int emptyCount = 0, fullCount;

    SEPARATOR;
    printf("1 DALIS - Salyga, informacija:\n\n");
    printf("UZDUOTIS nr. 2, VARIANTAS 8.\n");
    printf("AUTORIUS: Nirtautas Sadauskas (2213798) | GRUPE: 2 | POGRUPIS: 2.\n");
    printf("SALYGA: Parasyti programa sprendziancia sudoku.\n");
    SEPARATOR;

    while(1){
        printf("Iveskite ivesties failo pavadinima: ");
        scanf("%s", FDATA_NAME);

        if((fd = fopen(FDATA_NAME, "r")) != NULL){
            if (fetchSudoku(sudokuBoard)){
                while(1){
                    SEPARATOR;
                    emptyCount = findEmptyCount(sudokuBoard);
                    fullCount = SUDOKU_SIZE * SUDOKU_SIZE - emptyCount;

                    printSudoku(sudokuBoard);
                    SEPARATOR;

                    printf("Iveskite isvesties failo pavadinima: ");
                    scanf("%s", FRESULT_NAME);

                    if(!strcmp(FDATA_NAME, FRESULT_NAME) || (fr = fopen(FRESULT_NAME, "w")) != NULL){

                        FSEPARATOR;
                        fprintf(fr, "1 DALIS - Salyga, informacija:\n\n");
                        fprintf(fr, "UZDUOTIS nr. 2, VARIANTAS 8.\n");
                        fprintf(fr, "AUTORIUS: Nirtautas Sadauskas (2213798) | GRUPE: 2 | POGRUPIS: 2.\n");
                        fprintf(fr, "SALYGA: Parasyti programa sprendziancia sudoku.\n");

                        FSEPARATOR;
                        fprintf(fr, "1.1. DALIS - Duomenys:\n\n");
                        fprintSudoku(sudokuBoard);
                        fprintf(fr, "\nTusciu langeliu: %d.\nUzpildytu langeliu: %d.\n", emptyCount, fullCount);

                        FSEPARATOR;
                        fprintf(fr, "2 DALIS - Sprendimas:\n\n");

                        printf("Programa dirba...\n");
                        if(solveSudoku(sudokuBoard)){
                            SEPARATOR;
                            printf("3 DALIS - Rezultatas:\n\nSudoku issprendziamas.\n");
                            printf("Zingsniu skaicius: %d.\n\n", stepCount);

                            FSEPARATOR;
                            fprintf(fr, "3 DALIS - Rezultatas:\n\nSudoku issprendziamas.\n");
                            fprintf(fr, "Zingsniu skaicius: %d.\n\n", stepCount);
                        } else {
                            SEPARATOR;
                            printf("3 DALIS - Rezultatas:\n\nSudoku nera issprendziamas.\n");
                            printf("Zingsniu skaicius: %d.\n\n", stepCount);

                            fprintf(fr, ".\n");
                            FSEPARATOR;
                            fprintf(fr, "3 DALIS - Rezultatas:\n\nSudoku nera issprendziamas.\n");
                            fprintf(fr, "Zingsniu skaicius: %d.\n\n", stepCount);
                        }

                        printf("Sudoku lenta po sprendimo:\n");
                        fprintf(fr, "Sudoku lenta po sprendimo:\n");
                        printSudoku(sudokuBoard);
                        fprintSudoku(sudokuBoard);
                        SEPARATOR;
                        FSEPARATOR;
                        break;
                    } else
                        printError(1);
                }
                break;
            }
        } else
            printError(4);
    }

    fclose(fr);
    fclose(fd);
    return 0;
}

bool fetchSudoku(int board[][SUDOKU_SIZE]){
    char c;
    for (int row = 0 ; row < SUDOKU_SIZE ; ++row){
        for (int col = 0 ; col < SUDOKU_SIZE ; ++col){
            c = fgetc(fd);
            if (c == EOF || !isdigit(c)){
                printError(0);
                return false;
            }
            board[row][col] = c - 48;
        }
        c = fgetc(fd);
    }

    if (!checkData(board)){
        printError(5);
        return false;
    }

    printf("Sudoku paimtas is failo sekmingai.\n");
    return true;
}

bool checkData(int board[][SUDOKU_SIZE]){
    int temp;

    for (int row = 0 ; row < SUDOKU_SIZE ; ++row){
        for (int col = 0 ; col < SUDOKU_SIZE ; ++col){
            temp = board[row][col];
            board[row][col] = 0;
            if (temp != 0 && !isValid(board, row, col, temp)){
                board[row][col] = temp;
                return false;
            }
            board[row][col] = temp;
        }
    }
    return true;
}

void printSudoku(int board[][SUDOKU_SIZE]){
    printf("-------------->Y\n");
    for(int row = 0 ; row < SUDOKU_SIZE ; ++row){
        if (row % 3 == 0 && row != 0)
            printf("------------\n");

        for (int col = 0 ; col < SUDOKU_SIZE ; ++col){
            if(col % 3 == 0)
                printf("|");

            printf("%d", board[row][col]);
        }
        NEWLINE;
    }
    printf("|\nV\nX\n");
}

void fprintSudoku(int board[][SUDOKU_SIZE]){
    fprintf(fr, "-------------->Y\n");
    for(int row = 0 ; row < SUDOKU_SIZE ; ++row){
        if (row % 3 == 0 && row != 0)
            fprintf(fr, "------------\n");

        for (int col = 0 ; col < SUDOKU_SIZE ; ++col){
            if(col % 3 == 0)
                fprintf(fr, "|");

            fprintf(fr, "%d", board[row][col]);
        }
        FNEWLINE;
    }
    fprintf(fr, "|\nV\nX\n");
}

bool solveSudoku(int board[][SUDOKU_SIZE]){
    int erow, ecol;

    if (!findEmpty(board, &erow, &ecol)){
        ++stepCount;
        fprintf(fr, ".\n%10d) Nebera tusciu langeliu.\n", stepCount);
        return true;
    }

    ++deepCount;
    for (int num = 1 ; num <= 9 ; ++num){

        if (backtrackCount != 0){
            fprintf(fr, ", BACKTRACK %d times.\n", backtrackCount);
            backtrackCount = 0;
        } else if (stepCount != 0)
            fprintf(fr, ".\n");

        ++stepCount;
        fprintf(fr, "%10d) ", stepCount);
        for(int i = 0 ; i < deepCount ; ++i)
            fprintf(fr, "-");
        fprintf(fr, "Dedamas numeris %d vietoje: (X = %d, Y = %d) ", num, erow + 1, ecol + 1);

        if (isValid(board, erow, ecol, num)){
            board[erow][ecol] = num;
            fprintf(fr, "- OK");
            if (solveSudoku(board))
                return true;
            board[erow][ecol] = 0;
        } else
            fprintf(fr, "- FAIL");
    }

    ++backtrackCount;
    --deepCount;
    return false;
}

bool findEmpty(int board[][SUDOKU_SIZE], int* erow, int *ecol){
    for (int row = 0 ; row < SUDOKU_SIZE ; ++row){
        for (int col = 0 ; col < SUDOKU_SIZE ; ++col){
            if (board[row][col] == 0){
                *erow = row;
                *ecol = col;
                return true;
            }
        }
    }
    return false;
}

int findEmptyCount(int board[][SUDOKU_SIZE]){
    int emptyCount = 0;

    for (int row = 0 ; row < SUDOKU_SIZE ; ++row){
        for (int col = 0 ; col < SUDOKU_SIZE ; ++col)
            if(board[row][col] == 0)
                ++emptyCount;
    }
    return emptyCount;
}

bool checkRow(int board[][SUDOKU_SIZE], int row, int num){
    for (int col = 0 ; col < SUDOKU_SIZE ; ++col){
        if (board[row][col] == num)
            return false;
    }
    return true;
}

bool checkCol(int board[][SUDOKU_SIZE], int col, int num){
    for (int row = 0 ; row < SUDOKU_SIZE ; ++row){
        if (board[row][col] == num)
            return false;
    }
    return true;
}

bool checkBlock(int board[][SUDOKU_SIZE], int row, int col, int num){
    int rowBlock = row / 3 * 3;
    int colBlock = col / 3 * 3;

    for (int row = rowBlock ; row <= rowBlock + 2 ; ++row){
        for (int col = colBlock ; col <= colBlock + 2 ; ++col){
            if (board[row][col] == num)
                return false;
        }
    }
    return true;
}

bool isValid(int board[][SUDOKU_SIZE], int row, int col, int num){
    if (checkRow(board, row, num) == false || checkCol(board, col, num) == false || checkBlock(board, row, col, num) == false)
        return false;
    return true;
}

void printError(int index){
    if (index < 0){
        printError(2);
    } else if (index >= (int) ERROR_ARR_SIZE){
        printError(3);
    } else
        printf("Klaida %d: %s", index, ERROR_CODE[index]);
}
