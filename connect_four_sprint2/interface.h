// interface.h - Header file for user interface functions in Connect Four game, Group LSC
#ifndef INTERFACE_H
#define INTERFACE_H

#include "game_logic.h" // Contains definitions for Cell, ROWS (6), and COLS (7)
void display_board(Cell board[ROWS][COLS]);
int get_player_input(Cell player);
void print_winner(Cell player);
void print_draw(void);
void print_welcome_message(void);
void choose_mode(void);
#endif
