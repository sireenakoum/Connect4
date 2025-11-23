// main.c - Entry point for Connect Four game, Group LSC
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "game_logic.h"
#include "interface.h"

#define PORT 12345
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    Cell board[ROWS][COLS];
    Cell current_player = PLAYER_A;  // Player A starts
    int column, winner_found = 0;

    // Initialize board and print welcome
    int is_bot;
    int difficulty;
    int networking;
    int server_fd, client_fd, sock_fd;
    struct sockaddr_in address, serv_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addrlen = sizeof(address);
    
    fillBoard(board);
    print_welcome_message();
    choose_mode(&is_bot, &difficulty, &networking);
    if (networking == 3) { //Server mode
       server_fd = socket(AF_INET, SOCK_STREAM, 0);
       memset(&address, 0, sizeof(address));
       address.sin_family = AF_INET;
       address.sin_addr.s_addr = INADDR_ANY;
       address.sin_port = htons(PORT);
       bind(server_fd, (struct sockaddr*)&address, sizeof(address));
       listen(server_fd, 3);
       printf("Waiting for a client to connect\n");
       client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
       printf("Client connected\n");
    }
    if (networking == 4) { //Client mode
       sock_fd = socket(AF_INET, SOCK_STREAM, 0);
       memset(&serv_addr, 0, sizeof(serv_addr));
       serv_addr.sin_family = AF_INET;
       serv_addr.sin_port = htons(PORT);
       if (argc == 2) // get IP of server from command line
           inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
       else // Server is on 127.0.0.1 (Same machine)
           inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
       connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
       printf("Connected to server\n");
    }
    display_board(board);

    // Game loop
    while (!boardFull(board)) {

     if (networking == 3) {
          //Send move over network
          ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE);
          if (bytes_read <= 0) break;
          buffer[bytes_read] = '\0';
          //Apply the move client for the Server/bot
          current_player = PLAYER_A;
          column = atoi(buffer);
          make_move(board, column + 1, current_player);
          //If no winner, bot makes a move and sends it over network
          if (check_win(board, current_player) == 0) {
             current_player = PLAYER_B;
             if (difficulty == 1) //Easy Mode
		column = make_move_bot(board);
             else if (difficulty == 2) {//Medium mode
                column = bot_medium_move(board);
             }
             else //if (difficulty == 3) //Hard mode
                column = bot_hard_move(board);

             int len = snprintf(buffer, sizeof(buffer), "%d", column);
             write(client_fd, buffer, len);
          }
      }
      else if (networking == 4) {
          // Get valid input from current player
          current_player = PLAYER_A;
          column = get_player_input(current_player);
          // Make the move
          make_move(board, column + 1, current_player);
          //Send player move to server
          int len = snprintf(buffer, sizeof(buffer), "%d", column);
          write(sock_fd, buffer, len);
          //Get move from server if there is no winner
          if (check_win(board, current_player) == 0) {
             ssize_t bytes_read = read(sock_fd, buffer, BUFFER_SIZE);
             if (bytes_read <= 0) break;
             buffer[bytes_read] = '\0';
             //Apply the move for the Server/bot
             current_player = PLAYER_B;
             column = atoi(buffer);
             make_move(board, column + 1, current_player);
          }
      }
      else if((is_bot == 1) && (current_player == PLAYER_B)){
         // Get valid input from bot then make the move
        if (difficulty == 1) //Easy Mode
	    column = make_move_bot(board);
        else if (difficulty == 2) {//Medium mode
            column = bot_medium_move(board);
        }
        else //if (difficulty == 3) //Hard mode
            column = bot_hard_move(board);
      }
      else {
        //Play against human on without networking
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
    
    //Close sockets
    if (networking == 3) {
       close(client_fd);
       close(server_fd);
    }
    if (networking == 4) {
       close(sock_fd);
    }

    return 0;
}
