#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include "colours.h"

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

#define AI 2
#define HUMAN 1
#define EMPTY 0

#define YELLOW 6
#define BLUE 1
#define WHITE 15
#define RED 4

long long evaluate_window(short window[4], short player);

struct minimax_return{
    short column;
    long long score;
};

void print_board(short** board){
    printf("\n");
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
    colourChange(CYAN);
}

short is_legal_move(short** board, short column){
    if(board[0][column] == 0 && (7 > column && column >= 0))
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

short handle_win(short** board) {
    short state = iswin(board);
    if (state == 2) {
        printf("\nThe computer has won. Resetting board...\n");
    } else if (state == 1) {
        printf("\nYou won! Resetting board...\n");
    } else if(state == 3){
        printf("\nIt's a draw. Resetting board...\n");
    }
    if(state>0){
        reset_board(board);
        sleep(3);
        return 1;
    }
    return 0;
}

long long evaluate_window(short window[4], short player) {
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
        return  10000000000;
    if (opponent_count == 4)
        return  -(10000000000);
    if (opponent_count == 0)
        return player_count * player_count * 100;
    if (player_count == 0)
        return -(opponent_count * opponent_count * 100);
    return player_count - opponent_count;
}
long long evaluate_board(short** board, short player){
    long long score = 0;
    short window[4];
    long long window_score;
    for (int i = 0; i < BOARD_HEIGHT; ++i) { //check rows
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i][j + k];
            }
            window_score = evaluate_window(window, player);
            if(llabs(window_score) >= 10000000000)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check columns
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i+k][j];
            }
            window_score = evaluate_window(window, player);
            if(llabs(window_score) >= 10000000000)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check upward diagonals
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i+k][j+k];
            }
            window_score = evaluate_window(window, player);
            if(llabs(window_score) >= 10000000000)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 3; i < BOARD_HEIGHT; ++i) { //check downward diagonals
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i-k][j+k];
            }
            window_score = evaluate_window(window, player);
            if(llabs(window_score) >= 10000000000)
                return window_score;
            score += window_score;
        }
    }
    return score;
}

short** copy_board(short** board){
    //returns pointer to a copy of values in board array
    short** new_board = malloc(sizeof(short*) * BOARD_HEIGHT); //allocate memory for board copy
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        new_board[i] = malloc(sizeof(short) * BOARD_WIDTH);
        memcpy(new_board[i], board[i], BOARD_WIDTH*sizeof(board[i][0]));
    }
    return new_board;
}

void free_board(short** board){
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        free(board[i]);
    }
    free(board);
}

struct minimax_return minimax(short** board, short depth, long long alpha, long long beta, short player){
    struct minimax_return best_move;
    best_move.column = -1;
    best_move.score = player == AI ? -10000000000 : 10000000000;
    short win_state = iswin(board);

    //stopping conditions
    if(win_state == 2){
        best_move.score =  10000000000;
        free_board(board);
        return best_move;
    }
    if(win_state == 1){
        best_move.score =  -10000000000;
        free_board(board);
        return best_move;
    }
    if(win_state == 3){
        best_move.score = 0;
        free_board(board);
        return best_move;
    }
    if(depth == 0){
        best_move.column = -1;
        best_move.score = evaluate_board(board, AI);
        free_board(board);
        return best_move;
    }

    long long score;
    for (short i = 0; i < BOARD_WIDTH; ++i) {
        if(is_legal_move(board, i) != 0){
            score = minimax(place_chip(copy_board(board), i, player), depth - 1, alpha, beta, 3 - player).score;
            if(player == AI){ //maximising
                if(score > best_move.score){
                    best_move.score = score;
                    best_move.column = i;
                }
                alpha = max(alpha, best_move.score);
                if(alpha >= beta)
                    break;
            }else{
                if(score < best_move.score){
                    best_move.score = score;
                    best_move.column = i;
                }
                beta = min(beta, best_move.score);
                if(alpha >= beta)
                    break;
            }
        }
    }
    free_board(board);
    return best_move;

}

int main() {
    //allocate memory for the board and initialize values to 0
    short** board = malloc(BOARD_HEIGHT*sizeof(short*));
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        board[i] = malloc(BOARD_WIDTH*sizeof(short));
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = EMPTY;
        }
    }
    short ai_move;
    short move;
    print_board(board);

    while(1){
        printf("\nEnter the index of the column in which you would like to play, in a range of 1-7\n");
        scanf("%hd", &move);
        move--;
        if(is_legal_move(board, move) == 1){ //if move is legal and chip was successfully placed
            place_chip(board, move, HUMAN);
            system("cls");
            print_board(board);
            if(iswin(board) > 0){
                if(handle_win(board) > 0)
                    NULL;
                continue;
            }
        }else{
            printf("That was not a valid move, please try again.\n");
            continue;
        }

        ai_move = minimax(copy_board(board), 9, -10000000000, 10000000000, AI).column;
        if(is_legal_move(board, ai_move)){
            place_chip(board, ai_move, AI);
            system("cls");
            print_board(board);
            if(iswin(board) > 0){
                if(handle_win(board) > 0){}
            }
        }
    }
    free_board(board);
}
