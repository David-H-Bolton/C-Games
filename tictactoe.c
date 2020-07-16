// tictactoe.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EMPTYCHAR ' '

int playerIsX, computerFirst, x, y, danger, turn,computerMove;
char board[3][3];  // holds X, O and space
char playerPiece, computerPiece;

int CountEmpty() {
    int total = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[x][y] == EMPTYCHAR)
                total++;
        }
    }
    return total;
}

void InitBoard(){
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            board[x][y] = EMPTYCHAR;
        }
    }
}

int CountRows(char piece, int * y,int winCheck) {
   int total= (board[0][*y] == piece) +
        (board[1][*y] == piece) +
        (board[2][*y] == piece);
   if (winCheck) return total;
   if ((board[0][*y] == EMPTYCHAR) ||
       (board[1][*y] == EMPTYCHAR) ||
       (board[2][*y] == EMPTYCHAR))
       return total;
   else
       return 0; // No space in row so no danger
}

int CountCols(char piece, int * x,int winCheck) {
    int total= (board[*x][0] == piece) +
        (board[*x][1] == piece) +
        (board[*x][2] == piece);
    if (winCheck) return total;
    if ((board[*x][0] == EMPTYCHAR) ||
        (board[*x][1] == EMPTYCHAR) ||
        (board[*x][2] == EMPTYCHAR))
        return total;
    else
        return 0; // No space in row so no danger
}

// x= 0 for top left to bottom right or 2 for bottom left to top right
int CountDiagonal(char piece, int * x,int winCheck) {
    int total;
    if (*x == 0)  // top left to bottom right
    {
        total = (board[0][0] == piece) + (board[1][1] == piece) + (board[2][2] == piece);
        if (winCheck) return total;
        if ((board[0][0] == EMPTYCHAR) || (board[1][1] == EMPTYCHAR) || (board[2][2] == EMPTYCHAR))
            return total;
        else
            return 0; // No space in diag so no danger
    }
    total = (board[0][2] == piece) + (board[1][1] == piece) + (board[2][0] == piece);
    if (winCheck) return total;
    if ((board[0][2] == EMPTYCHAR) || (board[1][1] == EMPTYCHAR) || (board[2][0] == EMPTYCHAR))
        return total;
    else
        return 0; // No space in diag so no danger
}


// return value (1=row,2=column,3=diagonal) and y,x if any row, column or diagonal has 2 enemy pieces
// By passing in the piece (X O), this code can also be used to choose the best movbe fo the computer
int InDanger(char piece, int * x, int * y) {

    for (*y = 0; *y < 3; (*y)++) {
        if (CountRows(piece, y,0)==2) return 1;
    }

    for (*x = 0; *x < 3; (*x)++) {
        if (CountCols(piece, x,0) == 2) return 2;
    }
    *x = 0;
    if (CountDiagonal(piece, x,0) == 2) return 3;
    *x = 2;
    if (CountDiagonal(piece, x,0) == 2) return 4;

    return 0; // no danger
}

// Get inpit 1-9
int GetInput() {
    char text[10];
    int value;
    while (1) {
        printf("Enter 1-9 or q to quit and press return: \n");
        fgets(text, 10, stdin);
        if (strlen(text) != 2) continue;
        if (text[0] == 'q'|| text[0]=='Q') return -1; // Q for Quit
        value = text[0]-'0';  // convert '1'-'9' to 1-9
        if (value < 1 || value > 9) continue;
        break;
    }
    return value;
}

int Random(int n) {
    return (rand() % n) + 1;
}

// Display board with numbers 1-9
void ShowBoard() {
    for (int y=0;y<3;y++){
        for (int x = 0; x < 3; x++) {
            int n = (y * 3) + x + 1; 
            switch (board[x][y]) {
                case ' ':printf(" ---  %d|",n); break;
                case 'X':printf(" XXX  %d|",n); break;
                case 'O':printf(" OOO  %d|",n); break;
                }

        }            
        printf("\n");
    }
    printf("\n");
}

void ShowMyMove(char piece, int x, int y) {
    printf("I place %c at %d\n", piece, (y*3)+x+1);
}

// Computer works out move it should do to stop danger
void PlacePiece(char piece, int x, int y, int danger) {
    switch (danger) {
    case 1: /*Rows*/
        for (x = 0; x < 3; x++) {
            if (board[x][y] == EMPTYCHAR)
            {
                board[x][y] = piece;
                ShowMyMove(piece, x, y);
                return;
            };
        }
        break;
    case 2: /*cols*/
        for (y = 0; y < 3; y++) {
            if (board[x][y] == EMPTYCHAR)
            {
                board[x][y] = piece;
                ShowMyMove(piece, x, y);
                return;
            };
        }
        break;
    case 3: /* top left diag*/
        if (board[0][0] == EMPTYCHAR) {
            board[0][0] = piece;
            ShowMyMove(piece, x, y);
            break;
        }
        if (board[1][1] == EMPTYCHAR) {
            board[1][1] = piece;
            ShowMyMove(piece, x, y);
            break;
        }
        if (board[2][2] == EMPTYCHAR) {
            board[2][2] = piece;
            ShowMyMove(piece, x, y);
            break;
        }
    case 4: /* top left diag*/
        if (board[2][0] == EMPTYCHAR) {
            board[2][0] = piece;
            ShowMyMove(piece, x, y);
            break;
        }
        if (board[1][1] == EMPTYCHAR) {
            board[1][1] = piece;
            ShowMyMove(piece, x, y);
            break;
        }
        if (board[0][2] == EMPTYCHAR) {
            board[0][2] = piece;
            ShowMyMove(piece, x, y);
            break;
        }
    }
}

// Computer finds a random empty space and plays it
void PlaceRandomPiece(char piece) {
    while (1) {
        int x = Random(3)-1;
        int y = Random(3) - 1;
        if (board[x][y] == EMPTYCHAR) {
            board[x][y] = piece;
            ShowMyMove(piece, x, y);
            break;
        }
  }
}

int IsGameFinished() {
    int xv = 0;
    int yv = 0;
    int * x = &xv;
    int * y = &yv;

    for (*y = 0; *y < 3; (*y)++) {        
        if (CountRows(playerPiece, y,1) == 3) return 1;
        if (CountRows(computerPiece, y,1) == 3) return 2;
    }

    for (*x = 0; *x < 3; (*x)++) {
        if (CountCols(playerPiece, x,1) == 3) return 1;        
        if (CountCols(computerPiece, x,1) == 3) return 2;
    }
    *x = 0;
    if (CountDiagonal(playerPiece, x,1) == 3) return 1;    
    if (CountDiagonal(computerPiece, x,1) == 3) return 2;
    *x = 2;    
    if (CountDiagonal(playerPiece, x,1) == 3) return 1;
    if (CountDiagonal(computerPiece, x,1) == 3) return 2;
    return 0; // not finished
}

// convert n )1-9) to x,y n = (y*3)+x+1 so reverse that
void GetXY(int n, int* x, int* y) {
    n--;
    *y = (n / 3);
    *x = n - (*y * 3);
}

int main()
{
    srand((int)time(NULL));  // Initialise Rand
    InitBoard();
    printf("Tic-Tac-Toe\n");
    playerIsX = Random(2) == 1;
 
    if (playerIsX)
        printf("You are playing as X\n");
    else
        printf("You are playing as O\n");
    playerPiece = (playerIsX) ? 'X' : 'O';
    computerPiece = (!playerIsX) ? 'X' : 'O';
    computerFirst = Random(2) == 1;
    computerMove = computerFirst;
    turn = 1;
    ShowBoard();
    while (1) {
        printf("Turn %d - ", turn);
        if (computerMove) 
            printf("My turn\n");
        else
            printf("Your turn\n");

        if (computerMove) {                
            // Checvk if winning move now possible
            danger = InDanger(computerPiece, &x, &y); // not danger but chance to win
            if (danger > 0) {
                PlacePiece(computerPiece, x, y, danger);
            }
            else{ // Now check if there's aposible winning move for oppo and try to block
                danger = InDanger(playerPiece, &x, &y);
                if (danger > 0) { // Better block it quick!
                    PlacePiece(computerPiece, x, y, danger);
                }
                else { // Any old move
                    PlaceRandomPiece(computerPiece);
                }
            }
        } // PCPCPCPCP
        else { // player turn
            int input = GetInput();
            if (input < 0) {
                printf("You quit. Loser!\n");
                exit(1);
            }
            GetXY(input, &x, &y);
            if (board[x][y] == ' ') {
              board[x][y] = playerPiece; 
            }
            else {
                printf("%d is an invalid move- try again.", input);                
                continue; // loop back again
            }
        }
        ShowBoard();
        int gameOver = IsGameFinished();
        if (gameOver > 0) { // someone won!
            if (gameOver == 1) {
                printf("You won on turn %d\n", turn);
            }
            else{
                printf("I won on turn %d\n", turn);
            }
            exit(0); // Stop program
        }
        turn++;        
        computerMove = 1 - computerMove;
        if (turn == 10) break; // No more moves. Must be a draw...
    }
    printf("Game finished in a draw.");
}