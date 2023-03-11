#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include "colours.h"

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6
#define AI 2
#define HUMAN 1
#define EMPTY 0

#define YELLOW 6
#define BLUE 1
#define WHITE 15
#define GREEN 2
#define RED 4

void print_board(short** board){
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        printf("%d  ", i+1);
    }
    printf("\n");
    for (int i = 0; i < 3*BOARD_WIDTH - 2; ++i) {
        printf("_");
    }
    printf("\n");
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if(board[i][j] == HUMAN)
                colourChange(RED);
            else if(board[i][j] == AI)
                colourChange(YELLOW);
            else colourChange(BLUE);

            printf("%hd  ", board[i][j]);
        }
        printf("\n");
    }
    colourChange(WHITE);
}

short is_legal_move(short** board, short column){
    if(board[0][column] == 0)
        return 1;
    else return 0;
}

short** place_chip(short** board, short column, short player){
    if(!is_legal_move(board, column)){ //check if column is full
        return 0;
    }
    for (int i = 0; i < BOARD_HEIGHT; ++i) { //search for current height of chips in given column
        if(board[i][column] != 0){ //add chip above last added chip
            board[i-1][column] = player;
            return board;
        }
    }
    board[BOARD_HEIGHT-1][column] = player; //add chip at bottom if no chips in column
    return board;
}

short isdraw(short** board){
    short draw = 1;
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        if(board[0][i] == 0)
            return 0;
    }
    return draw;
}

short iswin(short** board){
    if(isdraw(board)){
        return 3;
    }
    for (int i = 0; i < BOARD_HEIGHT; ++i) { //check rows
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            if (board[i][j] == board[i][j + 1] && board[i][j + 1] == board[i][j + 2] && board[i][j + 2] == board[i][j + 3] && board[i][j] != 0) {
                return board[i][j];
            }
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check columns
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[i][j] == board[i + 1][j] && board[i+1][j] == board[i + 2][j] && board[i + 2][j] == board[i + 3][j] && board[i][j] != 0) {
                return board[i][j];
            }
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check upward diagonals
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            if(board[i][j] == board[i+1][j+1] && board[i+1][j+1] == board[i+2][j+2] && board[i+2][j+2] == board[i+3][i+3] && board[i][j] != 0){
                return board[i][j];
            }
        }
    }
    for (int i = 3; i < BOARD_HEIGHT; ++i) { //check downward diagonals
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if(board[i][j] == board[i-1][j+1] && board[i-1][j+1] == board[i-2][j+2] && board[i-2][j+2] == board[i-3][i+3] && board[i][j] != 0){
                return board[i][j];
            }
        }
    }
    return 0;
}

short** reset_board(short** board){
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = 0;
        }
    }
    return board;

}

void handle_win(short** board) {
    short state = iswin(board);
    if (state == 2) {
        printf("\nThe computer has won. Resetting board...\n");
        reset_board(board);
    } else if (state == 1) {
        printf("\nYou won! Resetting board...\n");
        reset_board(board);
    } else if(state == 3){
        printf("\nIt's a draw. Resetting board...\n");
        reset_board(board);
    }
}

int evaluate_window(short window[4], short player) {
    short opponent = 3 - player;
    short player_count = 0;
    short opponent_count = 0;
    for (int i = 0; i < 4; ++i) { //count chips in window
        if (window[i] == player) {
            player_count++;
        } else if (window[i] == opponent) {
            opponent_count++;
        }
    }
    if (player_count == 4)
        return 1e10;
    if (opponent_count == 4)
        return -(1e10);
    if (opponent_count == 0) {
        return player_count * 1000;
    }
    return player_count - opponent_count;
}
int evaluate_board(short** board, short player){
    int score = 0;
    short window[4];
    int window_score;
    for (int i = 0; i < BOARD_HEIGHT; ++i) { //check rows
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i][j + k];
            }
            window_score = evaluate_window(&window, player);
            if(abs(window_score) > 9e9)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check columns
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i+k][j];
            }
            window_score = evaluate_window(&window, player);
            if(abs(window_score) > 9e9)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check upward diagonals
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i+k][j+k];
            }
            window_score = evaluate_window(&window, player);
            if(abs(window_score) > 9e9)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 3; i < BOARD_HEIGHT; ++i) { //check downward diagonals
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i+k][j-k];
            }
            window_score = evaluate_window(&window, player);
            if(abs(window_score) > 9e9)
                return window_score;
            score += window_score;
        }
    }
    return score;
}

int get_board_score(short** board, short player){
    return evaluate_board(board, player) - evaluate_board(board, 3 - player);
}

short minimax(short** board, short depth, int alpha, int beta, short ai_turn){

}

int main() {
    //allocate memory for the board and initialize values to 0
    short** board = malloc(BOARD_HEIGHT*sizeof(short*));
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        board[i] = malloc(BOARD_WIDTH*sizeof(short));
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = 0;
        }
    }
    short turn = HUMAN;
    short move;
    srand(NULL);
    while(1){
        if(turn == HUMAN){ //human plays
            print_board(board);
            printf("\n\nEnter the index of the column in which you would like to play, in a range of 1-7\n");
            scanf("%hd", &move);
            move--;
            if(move >= 0 && move <= 6 && place_chip(board, move, HUMAN) != 0){
                printf("\n%d\n", evaluate_board(board, HUMAN));
                handle_win(board);
                turn = AI;
            }
            else{
                printf("That was not a valid move, please try again.\n");
                continue;
            }
        }
        if(turn == AI){
            move = rand()%7;
            printf("%hd\n", move);
            if(board[0][move] == 0) {
                place_chip(board, move, AI);
                handle_win(board);
                turn = HUMAN;
            }


        }
    }
}