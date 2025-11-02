// main.c - Entry point for Connect Four game, Group LSC
#include <stdio.h>
#include "game_logic.h"
#include "interface.h"

int main(void) {
    Cell board[ROWS][COLS];
    Cell current_player = PLAYER_A;  // Player A starts
    int column, winner_found = 0;

    // Initialize board and print welcome
    int is_bot;
    int difficulty;
    fillBoard(board);
    print_welcome_message();
    choose_mode(&is_bot, &difficulty);
    display_board(board);

    // Game loop
    while (!boardFull(board)) {

     if((is_bot == 1) && (current_player == PLAYER_B)){
         // Get valid input from bot then make the move
        if (difficulty == 2) {//Medium mode
          bot_medium_move(board);
        }
        else //if (difficulty == 1) //Easy mode
            make_move_bot(board);  
      }
      else {
        // Get valid input from current player
        column = get_player_input(current_player);
        // Make the move
        make_move(board, column + 1, current_player);
      }
       

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
