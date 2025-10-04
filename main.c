// main.c - Entry point for Connect Four game, Group LSC
#include <stdio.h>
#include "game_logic.h"
#include "interface.h"

int main(void) {
    Cell board[ROWS][COLS];
    Cell current_player = PLAYER_A;  // Player A starts
    int column, winner_found = 0;

    // Initialize board and print welcome
    fillBoard(board);
    print_welcome_message();
    display_board(board);

    // Game loop
    while (!boardFull(board)) {
        // Get valid input from current player
        column = get_player_input(current_player);

        // Make the move
        make_move(board, column + 1, current_player);
        display_board(board);

        // Check win
        if (check_win(board, current_player)) {
            print_winner(current_player);
            winner_found = 1;
            break;
        }

        // Switch players
        current_player = (current_player == PLAYER_A) ? PLAYER_B : PLAYER_A;
    }

    // If no winner, it's a draw
    if (!winner_found) {
        print_draw();
    }

    return 0;
}
