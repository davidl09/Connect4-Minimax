import math
from numpy import array, copy

height = 6
width = 7
EMPTY = 0

def iswin(board, player):
    if list_legal_moves(board) == []:
        return 'draw'
    for i in range(height):  #check rows, works
        for j in range(width - 3):
            if all(board[i][j+k] == player for k in range(4)):
                return [(i, j+k) for k in range(4)]

    for j in range(width):
        for i in range(height - 3):
            if all(board[i+k][j] == player for k in range(4)):
                return [(i+k, j) for k in range(4)]

    
    for i in range(height - 3):  #check upward diagonals
        for j in range(width - 3):
            if all(board[i+k][j+k] == player for k in range(4)):
                return [(i+k, j+k) for k in range(4)]
    
    for i in range(3, height):  #check upward diagonals
        for j in range(width - 3):
            if all(board[i-k][j+k] == player for k in range(4)):
                return [(i-k, j+k) for k in range(4)]
    return False

def list_legal_moves(board):
    moves = []
    for col in range(width):
        if board[0][col] == 0:
            moves.append(col)
    return moves


def get_best_move(board, ai_turn, depth):
    valid_moves = list_legal_moves(board)
    col_scores = []
    for move in valid_moves:
        col_scores.append((move, minimax(add_chip([row[:] for row in board], move, ai_turn), 2, -(10**10), 10**10, not ai_turn)))
    scores = sum([score for col, score in [colscore for move, colscore in col_scores]])
    avg_score = scores/len(col_scores)
    n_col_scores = []
    for move, colscore in col_scores:
        if avg_score < colscore[1]:
            n_col_scores.append((move, minimax(add_chip([row[:] for row in board], move, ai_turn), 4, -(10**10), 10**10, not ai_turn)))
    print(n_col_scores)



def minimax(board, depth, alpha, beta, ai_turn):
    valid_moves = list_legal_moves(board)
    if len(valid_moves) == 0:
        return None, 0
    if iswin(board, 2):
        return None, 10**(4 + depth)
    if iswin(board, 1):
        return None, -(10**(4 + depth))
    if depth == 0:
        return None, board_score(board, 2)
    
    column = None
    score = None
    if ai_turn:  #maximizing score
        score = -math.inf
        for col in valid_moves:
            board_copy = add_chip(board, col, ai_turn)
            if board_copy == False:
                continue
            new_score = minimax(board_copy, depth-1, alpha, beta, not ai_turn)[1]
            if new_score > score:
                score = int(new_score)
                column = int(col)
            alpha = max(alpha, score)
            if alpha >= beta:
                break
        return column, score
    else:  #minimizing score:
        score = math.inf
        for col in valid_moves:
            board_copy = add_chip(board, col, ai_turn)
            if board_copy == False:
                continue
            new_score = minimax(board_copy, depth-1, alpha, beta, not ai_turn)[1]
            if new_score < score:
                score = int(new_score)
                column = int(col)
            beta = min(beta, score)
            if alpha >= beta:
                break
        return column, score
    

def board_score(board, player):
    return evaluate_board(board, player) - evaluate_board(board, 3 - player)

def evaluate_board(board, player):
    score = 0

    for i in range(height):  #check rows, works
        for j in range(width - 3):
            window = [board[i][j+k] for k in range(4)]
            window_score = eval_window(window, player)
            if j in range(1, 3):
                window_score *= 2
            else:
                score += window_score

    for j in range(width):  #check columns, works
        for i in range(height - 3):
            window = [board[i+k][j] for k in range(4)]
            window_score = eval_window(window, player)
            if j in range(4, 5):
                window_score *= 2            
            else:
                score += window_score
    
    for i in range(height - 3):  #check upward diagonals
        for j in range(width - 3):
            window = [board[i+k][j+k] for k in range(4)]
            window_score = eval_window(window, player)
            if j in range(1, 3):
                window_score *= 2
            else:
                score += window_score
    
    for i in range(3, height):  #check downward diagonals
        for j in range(width - 3):
            window = [board[i-k][j+k] for k in range(4)]
            window_score = eval_window(window, player)
            if j in range(1, 3):
                window_score *= 2
            else:
                score += window_score

    return score

def eval_window(window, player):
    opp_player = 3 - player
    if window.count(player) == 4:
        return 10**10
    if window.count(opp_player) == 4:
        return -10**10
    if window.count(opp_player) == 0:
        return 5**window.count(player)
    return window.count(player) - window.count(opp_player)


def add_chip(board, column, ai_turn):
    board = [row[:] for row in board]
    if board[0][column] != 0:
        return False
    else:
        i=1
        while True:
            if i == 6:
                board[i-1][column] = int(ai_turn) + 1
                return board
            if board[i][column] != 0:
                board[i-1][column] = int(ai_turn) + 1
                return board
            i+=1
