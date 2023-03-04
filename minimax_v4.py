import numpy as np
import random
import math
import tkinter as tk

height = 6
width = 7
EMPTY = 0
RED = 1
YELLOW = 2

def iswin(board, player):
    for i in range(height):  #check rows, works
        for j in range(width - 3):
            if all(board[i][j+k] == player for k in range(4)):
                return True

    for j in range(width):
        for i in range(height - 3):
            if all(board[i+k][j] == player for k in range(4)):
                return True

    
    for i in range(height - 3):  #check upward diagonals
        for j in range(width - 3):
            if all(board[i+k][j+k] == player for k in range(4)):
                return True
    
    for i in range(3, height):  #check upward diagonals
        for j in range(width - 3):
            if all(slots == player for slots in [board[i-k][j+k] for k in range(4)]):
                return True
    return False

def list_legal_moves(board):
    moves = []
    for col in range(width):
        if board[0][col] == 0:
            moves.append(col)
    return moves


def minimax(board, depth, ai_turn=False):
    valid_moves = list_legal_moves(board)
    if len(valid_moves) == 0:
        return None, 0
    if iswin(board, 2):
        return None, 10000
    if iswin(board, 1):
        return None, -10000
    if depth == 0:
        return None, board_score(board, ai_turn+1)
    score = -math.inf
    high_score_col = random.choice(valid_moves)
    if ai_turn:  #maximizing score
        for col in list_legal_moves(board):
            board_copy = board
            new_score = (col, minimax(add_chip(board_copy, col, ai_turn), depth-1, not ai_turn))
            if new_score >= score:
                score = new_score
                high_score_col = col
        return high_score_col, board_score()

def board_score(board, player):
    return evaluate_board(board, player) - evaluate_board(board, 3 - player)

def evaluate_board(board, player):
    score = 0

    for i in range(height):  #check rows, works
        for j in range(width - 3):
            window = [board[i][j+k] for k in range(4)]
            if window.count(player) == 4:
                return 5000
            if window.count(3 - player) == 4:
                return -5000
            score += math.ceil((2+window.count(player))**2  + window.count(EMPTY))

    for j in range(width):  #check columns, works
        for i in range(height - 3):
            window = [board[i+k][j] for k in range(4)]
            if window.count(player) == 4:
                return 5000
            if window.count(3 - player) == 4:
                return -5000
            score += math.ceil((2+window.count(player))**2  + window.count(EMPTY))
    
    for i in range(height - 3):  #check upward diagonals
        for j in range(width - 3):
            window = [board[i+k][j+k] for k in range(4)]
            if window.count(player) == 4:
                return 5000
            if window.count(3 - player) == 4:
                return -5000
            score += math.ceil((2+window.count(player))**2  + window.count(EMPTY))
    
    for i in range(3, height):  #check upward diagonals
        for j in range(width - 3):
            window = [board[i-k][j+k] for k in range(4)]
            if window.count(player) == 4:
                return 5000
            if window.count(3 - player) == 4:
                return -5000
            score += math.ceil((2+window.count(player))**2  + window.count(EMPTY))

    return score


def add_chip(board, column, ai_turn):
    if board[0][column] != 0:
        return False
    else:
        i=1
        while True:
            if board[i][column] != 0 or i == 6:
                board[i-1][column] = int(ai_turn) + 1
                return board
            i+=1
            


class Connect4GUI:
    def __init__(self, rows=6, cols=7):
        self.rows = rows
        self.cols = cols
        self.current_player = 1
        self.board = [[0 for col in range(cols)] for row in range(rows)]

        # Create the root window
        self.root = tk.Tk()
        self.root.title("Connect4")
        self.root.size()
        # Create a frame to hold the buttons
        self.frame = tk.Frame(self.root)
        self.frame.pack(fill=tk.BOTH, expand=True)

        # Create a 2-dimensional list of buttons
        self.buttons = [[tk.Button(self.frame, width=3, height=1, bg='white', activebackground='blue', bd=0,
                                    command=lambda col=col:self.play_turn(col)) 
                         for col in range(cols)] for row in range(rows)]
        
        # Configure the grid to make the buttons large and round
        self.frame.columnconfigure(list(range(cols)), weight=1, uniform="column")
        self.frame.rowconfigure(list(range(rows)), weight=1, uniform="row")
        for row in range(rows):
            for col in range(cols):
                self.buttons[row][col].grid(row=row, column=col, padx=5, pady=5)
                self.buttons[row][col].config(width=8, height=8, borderwidth=0, highlightthickness=0,
                                               relief="flat", bg="white", activebackground='blue',
                                               disabledforeground="black", highlightcolor="white",
                                               font=("Arial", 20), pady=0)

        # Start the main loop
        self.root.mainloop()

    def play_turn(self, col):
        for row in range(self.rows-1, -1, -1):
            if self.board[row][col] == 0:
                self.board[row][col] = self.current_player
                self.buttons[row][col].config(bg='red' if self.current_player == 1 else 'yellow', state='disabled')
                print(iswin(self.board, self.current_player))
                print(np.array(self.board))
                print(board_score(self.board, 1))
                self.current_player = 2 if self.current_player == 1 else 1
                break


board = Connect4GUI()
#print(evaluate_window([1, 0, 2, 2], 2))
