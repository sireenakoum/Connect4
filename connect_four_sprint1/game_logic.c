// game_logic.c - Implements the core logic for a two-player Connect Four game, Group LSC
#include <stdio.h>
#include "game_logic.h"

//Initializes the game board by setting all cells to EMPTY
void fillBoard(Cell board[ROWS][COLS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            board[i][j] = EMPTY;
        }
    }
}

//Handles a move by placing the player's token into the specified column
void make_move(Cell board[ROWS][COLS], int column, Cell player){
    while(column <= 0 || column > COLS){
        printf("Invalid column. Please select from 1-7:\n");
        scanf("%d", &column);
    }
    // Ensure the selected column is not full
    while(board[0][column-1] != EMPTY){
        printf("The column you selected is full. Please select another:\n");
        scanf("%d", &column);
    }
    // Place the token in the lowest available cell in the selected column
    for(int i = ROWS-1; i >= 0; i--){
       if(board[i][COLS] == EMPTY){
           board[i][COLS] = player;
           break;
       }
    }
}

//Checks whether the given player has achieved a winning condition.
int check_win(Cell board[ROWS][COLS],Cell player){
     //Horizontal check
    for(int i = 0 ; i < ROWS; i++){
        for(int j = 0;j <= COLS-4; j++){
            if(board[i][j]==player
            &&board[i][j+1]==player
            &&board[i][j+2]==player
            &&board[i][j+3]==player){
               return 1;
                }
            }
        }
        //Vertical check
    for(int i = 0 ; i <= ROWS-4; i++){
        for(int j=0; j < COLS; j++){
            if(board[i][j]==player
            &&board[i+1][j]==player
            &&board[i+2][j]==player
            &&board[i+3][j]==player){
               return 1;
                }
            }
        }
            //Diagonal (top-left to bottom-right)
      for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 0; j <= COLS - 4; j++) {
            if (board[i][j] == player &&
                board[i+1][j+1] == player &&
                board[i+2][j+2] == player &&
                board[i+3][j+3] == player)
                return 1;
        }
    }
        //Diagonal (top-right to bottom-left)
     for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 3; j < COLS; j++) {
            if (board[i][j] == player &&
                board[i+1][j-1] == player &&
                board[i+2][j-2] == player &&
                board[i+3][j-3] == player)
                return 1;
        }
    }
        return 0; // No win found
        
}

//Checks whether the game board is full. Returns 1 if the board is full, 0 otherwise.
int boardFull(Cell board[ROWS][COLS]){
        for(int i = 0 ; i < ROWS; i++){
            for(int j=0; j < COLS; j++){
                if(board[i][j] == EMPTY){
                return 0;
                }
            }
        }
        return 1;
}

//Determines if the game is a draw(i.e, full board and no winner). Returns 1 if the game is a draw, 0 otherwise.
int is_draw(Cell board[ROWS][COLS]){
    return boardFull(board);
}