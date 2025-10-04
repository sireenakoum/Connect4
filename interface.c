// interface.c - User Interface module for Connect Four game, Group LSC
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

//Prints the welcome message as well as the game instructions to the user
void print_welcome_message(void) {
    printf("Welcome to Connect Four!\n");
    printf("Players take turns dropping checkers into the columns.\n");
    printf("First to connect four in a row (horizontally, vertically, or diagonally) wins!\n");
    printf("Enter column numbers between 1 and 7.\n\n");
}

//Displays the current state of the game board
void display_board(Cell board[ROWS][COLS]) {
    // Iterate over each cell in the board and print 'A' for player A, 'B' for player B, or '.' for unoccupied cell
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            char symbol = '.';
            if (board[i][j] == PLAYER_A) symbol = 'A';
            else if (board[i][j] == PLAYER_B) symbol = 'B';
            printf(" %c", symbol);
        }
        printf("\n");
    }
    // Print column numbers from 1 to 7
    printf(" 1 2 3 4 5 6 7\n");
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
        if (column < 0 || column >= COLS) {
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
