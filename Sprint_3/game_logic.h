// game_logic.h - Header file for Connect Four game logic functions and definitions, group LSC
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
// Enumeration representing the state of a cell on the board
typedef enum{
EMPTY, // Cell is empty
PLAYER_A, // Cell is occupied by Player A
PLAYER_B // Cell is occupied by Player B
}Cell;

// Board dimensions: 6x7 grid 
#define ROWS 6
#define COLS 7
void fillBoard(Cell board[ROWS][COLS]);
void make_move(Cell board[ROWS][COLS],int column, Cell player);
void make_move_bot(Cell board[ROWS][COLS]);
int can_win_next(Cell board[ROWS][COLS], Cell player);
int can_block_opponent(Cell board[ROWS][COLS], Cell opponent);
int check_win(Cell board[ROWS][COLS],Cell player);
int boardFull(Cell board[ROWS][COLS]);
int is_draw(Cell board[ROWS][COLS]);
int evaluate_window(Cell window[4], Cell bot, Cell player);
int evaluate_board(Cell board[ROWS][COLS], Cell bot, Cell player);
int bot_medium_move(Cell board[ROWS][COLS]);
#endif
