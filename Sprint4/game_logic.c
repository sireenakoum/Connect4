// game_logic.c - Implements the core logic for a two-player Connect Four game, Group LSC
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "game_logic.h"

#define MAX_DEPTH 6   // depth for hard bot minimax

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
       if(board[i][column-1] == EMPTY){
           board[i][column-1] = player;
           break;
       }
    }
}
int make_move_bot(Cell board[ROWS][COLS]){
    int random_col;
 // Pick random non-full column
    do{
        random_col=rand()%COLS;
    } while( board[0][random_col]!=EMPTY);

    // Drop bot token in lowest empty cell
    for(int i=ROWS -1; i>=0;i--){
        if(board[i][random_col]==EMPTY){
            board[i][random_col]=PLAYER_B;
            break;
        }
    }
    return random_col;
}

int can_win_next(Cell board[ROWS][COLS], Cell player){
    int canWin = -1;
    for (int j = 0; j < COLS; j++) {
        //Find the last empty cell in the column
        if (board[0][j] == EMPTY) {//places are still available in column
            for (int i = ROWS - 1; i >= 0; i--) {
                if (board[i][j] == EMPTY) {
                    //Test the win
                    board[i][j] = player;
                    canWin = check_win(board, player);
                    board[i][j] = EMPTY;
                    if (canWin) {
                      return j;
                    }
                }
            }
        }
    }
    return -1;
}

int can_block_opponent(Cell board[ROWS][COLS], Cell opponent){
   //Check Vertical if 3 consecutive
   for (int j = 0; j < COLS; j++) {
      int row = 0;
      while ((row < ROWS-3) && (board[row][j] == EMPTY)) { //Search for non-empty cell in a column
          row++;
      }
      if ((board[row][j] != EMPTY) 
      && (board[row][j] == opponent) 
      && (board[row+1][j] == opponent) 
      && (board[row+2][j] == opponent)) {
          return j;
      }
   }
   
   //Check Horizontal if 3 consecutive
   for (int i = ROWS-1; i >= 0; i--) {
      for (int first = 0; first < COLS - 3; first++) { //Check every 4 consecutive columns per row
          //count the number of cells for opponent with row below being non-empty
          int count = 0;
          if ((i == ROWS-1) || ((board[i+1][first] != EMPTY) && (board[i+1][first+1] != EMPTY) && (board[i+1][first+2] != EMPTY) && (board[i+1][first+3] != EMPTY))) {
            if (board[i][first] == opponent)
              count++;
            if (board[i][first+1] == opponent)
              count++;
            if (board[i][first+2] == opponent)
              count++;
            if (board[i][first+3] == opponent)
              count++;
          }
          if (count == 3) { //Opponent can win, needs to be blocked
             //Search for the empty cell
             for (int j = first; j < first + 4; j++) {
                 if (board[i][j] == EMPTY) {
                    return j;
             }
          }
        }
      }
   }
   
   //Check if diagonal (top-left to bottom-right) has 3 consecutive
   for (int i = 0; i <= ROWS - 4; i++) {
      for (int first = 0; first < COLS - 3; first++) {
        int count = 0;
        if (board[i][first] == opponent)
          count++;
        if (board[i+1][first+1] == opponent)
          count++;
        if (board[i+2][first+2] == opponent)
          count++;
        if (board[i+3][first+3] == opponent)
          count++;
        if (count == 3) { //check which column has empty cell and if cell below is non-empty
           if ((board[i][first] == EMPTY) && (board[i+1][first] != EMPTY)) 
              return first;
           if ((board[i+1][first+1] == EMPTY) && (board[i+2][first+1] != EMPTY)) 
              return first+1;
           if ((board[i+2][first+2] == EMPTY) && (board[i+3][first+2] != EMPTY)) 
              return first+2;
           if ((board[i+3][first+3] == EMPTY) && ( (i+4 == ROWS) || (board[i+4][first+3] != EMPTY))) 
              return first+3;
        }
      }
   }

   //Check if diagonal (bottom-left to top-right) has 3 consecutive
   for (int i = ROWS-1; i >= ROWS - 4 ; i--) {
      for (int first = 0; first < COLS - 3; first++) {
        int count = 0;
        if (board[i][first] == opponent)
          count++;
        if (board[i-1][first+1] == opponent)
          count++;
        if (board[i-2][first+2] == opponent)
          count++;
        if (board[i-3][first+3] == opponent)
          count++;
        if (count == 3) { //check which column has empty cell and if cell below is non-empty
           if ((board[i][first] == EMPTY) && ( (i+1 == ROWS) || (board[i+1][first] != EMPTY))) 
              return first;
           if ((board[i-1][first+1] == EMPTY) && (board[i][first+1] != EMPTY)) 
              return first+1;
           if ((board[i-2][first+2] == EMPTY) && (board[i-1][first+2] != EMPTY)) 
              return first+2;
           if ((board[i-3][first+3] == EMPTY) && (board[i-2][first+3] != EMPTY)) 
              return first+3;
        }
      }
   }

   return -1;
}

//Checks whether the given player has achieved a winning condition.
int check_win(Cell board[ROWS][COLS], Cell player){
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
                board[i+3][j+3] == player) {
                return 1;
              }
        }
    }
        //Diagonal (top-right to bottom-left)
     for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 3; j < COLS; j++) {
            if (board[i][j] == player &&
                board[i+1][j-1] == player &&
                board[i+2][j-2] == player &&
                board[i+3][j-3] == player) {
                return 1;
              }
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

// Evaluates a 4-cell segment of the board
int evaluate_window(Cell window[4], Cell bot, Cell player) {
    int botCount = 0, playerCount = 0, emptyCount = 0;

    for (int i = 0; i < 4; i++) {
        if (window[i] == bot) botCount++;
        else if (window[i] == player) playerCount++;
        else emptyCount++;
    }

    // === Scoring rules ===
    // The values below are weights that represent how strong or risky a position is:
    // +1000: winning position for the bot (4 in a row)
    // +5: strong advantage, bot has 3 in a row and can win next turn
    // +2: moderate setup, bot has 2 connected pieces and room to grow
    // -5: immediate danger, opponent (player) has 3 in a row, bot must block this
    // 0: neutral or mixed situation
    if (botCount == 4) return 1000;                          // winning window
    else if (botCount == 3 && emptyCount == 1) return 5;     // bot close to winning
    else if (botCount == 2 && emptyCount == 2) return 2;     // bot building potential
    else if (playerCount == 3 && emptyCount == 1) return -5; // opponent (player) about to win
    else return 0;                                           // neutral
}

// Evaluates the entire board and returns a score
int evaluate_board(Cell board[ROWS][COLS], Cell bot, Cell player) {
    int score = 0;
    Cell window[4];

    // Center column control
    int center = COLS / 2;
    int centerCount = 0;
    for (int r = 0; r < ROWS; r++)
        if (board[r][center] == bot) centerCount++;
    score += centerCount * 2;

    // Horizontal evaluation
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r][c + i];
            score += evaluate_window(window, bot, player);
        }
    }

    // Vertical evaluation
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS - 3; r++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r + i][c];
            score += evaluate_window(window, bot, player);
        }
    }

    // Diagonal (top-left to bottom-right)
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r + i][c + i];
            score += evaluate_window(window, bot, player);
        }
    }

    // Diagonal (top-right to bottom-left)
    for (int r = 3; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r - i][c + i];
            score += evaluate_window(window, bot, player);
        }
    }

    return score;
}
// bot_medium_move(): Medium bot decision logic integrating Win, Block, and Heuristic Evaluation
int bot_medium_move(Cell board[ROWS][COLS]) {
   // Define players - bot is always PLAYER_B, human is always PLAYER_A
    Cell bot = PLAYER_B;
    Cell player = PLAYER_A;

    int bestCol = -1;
    int bestScore = -100000;

    // Try to Win immediately
    int winCol = can_win_next(board, bot);
    if(winCol != -1) {
        make_move(board, winCol + 1, bot);
        return winCol;
    }

    // Try to Block opponent's immediate win
    int blockCol = can_block_opponent(board, player);
    if(blockCol != -1) {
        make_move(board, blockCol + 1, bot);
        return blockCol;
    }

    // Otherwise: evaluate all possible moves
    for(int c = 0; c < COLS; c++) {
        if(board[0][c] != EMPTY) continue;

        // Find the lowest empty row
        int r = -1;
        for(int i = ROWS - 1; i >= 0; i--) {
            if(board[i][c] == EMPTY) {
                r = i;
                break;
            }
        }
        // Simulate placing bot piece
        board[r][c] = bot;
        int score = evaluate_board(board, bot, player);
        board[r][c] = EMPTY;

        // Keep track of best column
        if(score > bestScore) {
            bestScore = score;
            bestCol = c;
        }
    }

    // Execute best move or fallback to random
    if(bestCol != -1) {
        make_move(board, bestCol + 1, bot);
    } else {
        make_move_bot(board);
        // Find which column was actually used
        for(int c = 0; c < COLS; c++) {
            if(board[ROWS-1][c] == bot) {
                bestCol = c;
                break;
            }
        }
    }
    return bestCol;
}

//This function returns whether the game reached its end state, serves as the stop condition of the minimax recursion
//Prevents wasting computations on finished games
//Provides immediate scoring for terminal positions
//Essential for establishing base cases in recursive search of minimax
int is_terminal(Cell board[ROWS][COLS]){
    if(check_win(board, PLAYER_A)){
        return 1;//human wins
    }
    if(check_win(board, PLAYER_B)){
        return 2;//bot wins
    }
if(boardFull(board)){
        return 3;//draw
    }
    return 0; //continue searching
}


//This function sees where the piece would fall and validates the move to see if it's a legal and possible move
//Provides row coordinate for piece placement in simulate_move()
int get_next_open_row(Cell board[ROWS][COLS],int col){
    //Validation of column
    if(col<0||col>=COLS)
        return -1;

    //Search for an empty cell from bottom to top
    for(int row=ROWS-1;row>=0;row--){
        if(board[row][col]==EMPTY){
            return row;
        }
    }
    //Column is full
    return -1;
}

//This fucntion temporarily places a piece for minimax exploration
//Creates what-if scenarios without affecting the actual game
//Allows testing moves in temporary board copies
void simulate_move(Cell board [ROWS][COLS],int col, Cell player){
    //Find where piece would land in this column   
    int row = get_next_open_row(board,col);
    //Place only if the row returned is a valid row
    if(row != -1){
        //Temporariy place player's piece for minimax evaluation
        board[row][col] = player;
    }
}

// MINIMAX with ALPHA-BETA PRUNING
// maximizingPlayer = 1 for bot (PLAYER_B)
// maximizingPlayer = 0 for human (PLAYER_A)
int minimax(Cell board[ROWS][COLS], int depth, int alpha, int beta, int maximizingPlayer) {
    int terminal = is_terminal(board);

    // Base cases
    if (depth == 0 || terminal != 0) {
        if (terminal == 2) return 1000000;    // bot win
        if (terminal == 1) return -1000000;   // human win
        if (terminal == 3) return 0;          // draw

        // Not terminal but depth reached, so heuristic
        return evaluate_board(board, PLAYER_B, PLAYER_A);
    }

    //Maximizing player(bot)
    if (maximizingPlayer) {
        int bestScore = -10000000;

        for (int col = 0; col < COLS; col++) {
            if (board[0][col] != EMPTY) continue;

            Cell temp[ROWS][COLS];
            for (int r = 0; r < ROWS; r++)
                for (int c = 0; c < COLS; c++)
                    temp[r][c] = board[r][c];

            simulate_move(temp, col, PLAYER_B);

            int score = minimax(temp, depth - 1, alpha, beta, 0);
            if (score > bestScore)
                bestScore = score;

            alpha = (alpha > bestScore) ? alpha : bestScore;

            // Beta cut, stop searching this branch
            if (beta <= alpha)
                break;
        }

        return bestScore;
    }

    //Minimizing player (human)
    else {
        int bestScore = 10000000;

        for (int col = 0; col < COLS; col++) {
            if (board[0][col] != EMPTY) continue;

            Cell temp[ROWS][COLS];
            for (int r = 0; r < ROWS; r++)
                for (int c = 0; c < COLS; c++)
                    temp[r][c] = board[r][c];

            simulate_move(temp, col, PLAYER_A);

            int score = minimax(temp, depth - 1, alpha, beta, 1);
            if (score < bestScore)
                bestScore = score;

            beta = (beta < bestScore) ? beta : bestScore;

            // Alpha cut
            if (beta <= alpha)
                break;
        }

        return bestScore;
    }
}
// Structure to pass data to each minimax worker thread at the root level
typedef struct {
    Cell board[ROWS][COLS];   // private copy of the board for this thread
    int col;                  // column to simulate
    int depth;                // remaining search depth
    int alpha;                // alpha value for minimax
    int beta;                 // beta value for minimax
    int maximizingPlayer;     // whose turn after this move (0 = human, 1 = bot)
    int score;                // result of the minimax evaluation
} MinimaxTask;

// Thread routine: simulate the bot's move in the given column and run minimax
void* minimax_thread(void* arg) {
    MinimaxTask* task = (MinimaxTask*)arg;

    // Simulate placing the bot's piece in this column on the private board
    simulate_move(task->board, task->col, PLAYER_B);

    // Evaluate the resulting position using minimax
    task->score = minimax(task->board,
                          task->depth,
                          task->alpha,
                          task->beta,
                          task->maximizingPlayer);
    return NULL;
}

int bot_hard_move(Cell board[ROWS][COLS]) {
    int bestCol = -1;
    int bestScore = -10000000;

    // Block opponent if they can win immediately
    int block = can_block_opponent(board, PLAYER_A);
    if (block != -1) {
        make_move(board, block + 1, PLAYER_B);
        return block;
    }

    // Win immediately if possible
    int win = can_win_next(board, PLAYER_B);
    if (win != -1) {
        make_move(board, win + 1, PLAYER_B);
        return win;
    }

    // Otherwise: evaluate all legal moves in parallel using threads
    pthread_t threads[COLS];
    MinimaxTask tasks[COLS];
    int taskCount = 0;

    // Create one worker thread per available column
    for (int col = 0; col < COLS; col++) {
        if (board[0][col] != EMPTY)
            continue;   // skip full columns

        // Prepare task for this column: copy the current board
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                tasks[taskCount].board[r][c] = board[r][c];
            }
        }
        tasks[taskCount].col = col;
        tasks[taskCount].depth = MAX_DEPTH - 1;
        tasks[taskCount].alpha = -10000000;
        tasks[taskCount].beta  = 10000000;
        tasks[taskCount].maximizingPlayer = 0;   // after bot moves, it's the human's turn
        tasks[taskCount].score = -10000000;

        // Launch a thread to evaluate this move using minimax
        pthread_create(&threads[taskCount], NULL, minimax_thread, &tasks[taskCount]);
        taskCount++;
    }

    // Wait for all threads to finish and pick the best result
    for (int i = 0; i < taskCount; i++) {
        pthread_join(threads[i], NULL);

        if (tasks[i].score > bestScore) {
            bestScore = tasks[i].score;
            bestCol = tasks[i].col;
        }
    }

    // If for some reason no move was found, do nothing
    if (bestCol != -1) {
        make_move(board, bestCol + 1, PLAYER_B);
    }

    return bestCol;
}