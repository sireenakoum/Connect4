// interface.c - User Interface module for Connect Four game, Group LSC
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"

//Prints the welcome message as well as the game instructions to the user
void print_welcome_message(void) {
    printf("Welcome to Connect Four!\n");
    printf("Players take turns dropping checkers into the columns.\n");
    printf("First to connect four in a row (horizontally, vertically, or diagonally) wins!\n");
    printf("Enter column numbers between 1 and 7.\n\n");
}

//Allows the user to choose the game mode
void choose_mode(int *is_bot, int *difficulty, int *networking){
    *is_bot = -1;
    printf("Choose \"1\" or \"2\" or \"3\" or \"4\" to select the game mode:\n");
    printf("1: Player vs Player\n");
    printf("2: Player vs Bot\n");
    printf("3: Network mode as Server\n");
    printf("4: Network mode as Client\n");
    int mode;
    scanf("%d", &mode);
    *networking = 0; 
    if ((mode == 3) || (mode == 4)) {
        *networking = mode;
        *is_bot = 1; //Does not matter the difficulty level since the bot is over a network.
    }

    //Allows the user to choose a difficulty level when playing against a bot
    if ((mode == 2) || (mode == 3)) {
       *is_bot = 1;
       printf("Choose\"1\", \"2\", or \"3\" to select difficulty level:\n");
       printf("1: Easy\n2: Medium\n3: Hard\n");
       scanf("%d", difficulty);
    }
}

//Displays the current state of the game board
void display_board(Cell board[ROWS][COLS]) {
    printf("\n");

    for (int r = 0; r < ROWS; r++) {
        printf(" |");
        for (int c = 0; c < COLS; c++) {

            if (board[r][c] == PLAYER_A)
                printf(" %s■%s | ", RED, RESET);      // Player A
            else if (board[r][c] == PLAYER_B)
                printf(" %s■%s | ", YELLOW, RESET);   // Player B
            else
                printf(" □ | ");                      // Empty
        }
        printf("\n\n");   // Blank line between rows
    }

    printf("   1    2   3    4    5    6    7\n\n");
}

//Prompts the current player for a column input and places it in the right cell position
int get_player_input(Cell player) {
    int column;
    char player_char = (player == PLAYER_A) ? 'A' : 'B';

    while (1) {
        printf("Player %c, enter column 1-7: ", player_char);
        // Validate input is a number
        if (scanf("%d", &column) != 1) {
            while (getchar() != '\n'); // clear buffer
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        // Validate input is within column range (1-7)
        if (column <= 0 || column > COLS) {
            printf("Invalid column. Please enter a number between 1 and 7.\n");
            continue;
        }
        return column-1;//subtract 1 since columns are numbered 1-7, but 2D array is 0-indexed
    }
}

//Displays the winner message for the given player
void print_winner(Cell player) {
    char player_char = (player == PLAYER_A) ? 'A' : 'B';
    printf("\nPlayer %c wins! Congratulations!\n", player_char);
}

//Displays a draw message when the board is full without a winner
void print_draw(void) {
    printf("\nIt's a draw! No more moves left.\n");
}
