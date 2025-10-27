#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define HUMAN 'X'
#define BOT 'O'
#define EMPTY ' '

void printBoard(char board[9]) {
    printf("\n");
    for (int r = 0; r < 3; ++r) {
        printf(" %c | %c | %c \n", board[r*3+0], board[r*3+1], board[r*3+2]);
        if (r < 2) printf("---+---+---\n");
    }
    printf("\n");
}

int isMovesLeft(char board[9]) {
    for (int i = 0; i < 9; ++i) if (board[i] == EMPTY) return 1;
    return 0;
}

int evaluate(char b[9]) {
    // Check rows, columns, diagonals for a winner
    for (int row = 0; row < 3; ++row) {
        if (b[row*3+0] == b[row*3+1] && b[row*3+1] == b[row*3+2]) {
            if (b[row*3+0] == BOT) return +10;
            else if (b[row*3+0] == HUMAN) return -10;
        }
    }
    for (int col = 0; col < 3; ++col) {
        if (b[0*3+col] == b[1*3+col] && b[1*3+col] == b[2*3+col]) {
            if (b[0*3+col] == BOT) return +10;
            else if (b[0*3+col] == HUMAN) return -10;
        }
    }
    // Diagonals
    if (b[0] == b[4] && b[4] == b[8]) {
        if (b[0] == BOT) return +10;
        else if (b[0] == HUMAN) return -10;
    }
    if (b[2] == b[4] && b[4] == b[6]) {
        if (b[2] == BOT) return +10;
        else if (b[2] == HUMAN) return -10;
    }
    return 0;
}

// Minimax with alpha-beta pruning
int minimax(char board[9], int depth, int isMaximizing, int alpha, int beta) {
    int score = evaluate(board);
    if (score == 10) return score - depth;    // prefer faster wins
    if (score == -10) return score + depth;   // prefer slower losses
    if (!isMovesLeft(board)) return 0;

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == EMPTY) {
                board[i] = BOT;
                int val = minimax(board, depth+1, 0, alpha, beta);
                board[i] = EMPTY;
                if (val > best) best = val;
                if (best > alpha) alpha = best;
                if (beta <= alpha) break; // beta cutoff
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == EMPTY) {
                board[i] = HUMAN;
                int val = minimax(board, depth+1, 1, alpha, beta);
                board[i] = EMPTY;
                if (val < best) best = val;
                if (best < beta) beta = best;
                if (beta <= alpha) break; // alpha cutoff
            }
        }
        return best;
    }
}

int findBestMove(char board[9]) {
    int bestVal = INT_MIN;
    int bestMove = -1;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == EMPTY) {
            board[i] = BOT;
            int moveVal = minimax(board, 0, 0, INT_MIN, INT_MAX);
            board[i] = EMPTY;
            if (moveVal > bestVal) {
                bestVal = moveVal;
                bestMove = i;
            }
        }
    }
    return bestMove; // returns index 0..8
}

int checkWinner(char board[9]) {
    int score = evaluate(board);
    if (score == 10) return 2;   // BOT wins
    if (score == -10) return 1;  // HUMAN (player X) wins
    if (!isMovesLeft(board)) return 0; // draw
    return -1; // no result yet
}

void initBoard(char board[9]) {
    for (int i = 0; i < 9; ++i) board[i] = EMPTY;
}

int getPlayerMove(char board[9], char playerChar) {
    int pos;
    while (1) {
        printf("Player %c, enter position (1-9): ", playerChar);
        if (scanf("%d", &pos) != 1) {
            while (getchar() != '\n'); // clear invalid input
            printf("Invalid input. Please enter a number 1-9.\n");
            continue;
        }
        if (pos < 1 || pos > 9) {
            printf("Position must be 1-9.\n");
            continue;
        }
        if (board[pos-1] != EMPTY) {
            printf("That cell is already taken. Choose another.\n");
            continue;
        }
        return pos-1;
    }
}

void showLegend() {
    printf("Board positions mapping:\n");
    printf(" 1 | 2 | 3 \n");
    printf("---+---+---\n");
    printf(" 4 | 5 | 6 \n");
    printf("---+---+---\n");
    printf(" 7 | 8 | 9 \n\n");
}

int main() {
    char board[9];
    initBoard(board);

    printf("=== Tic Tac Toe ===\n");
    showLegend();

    int mode = 0;
    while (1) {
        printf("Choose mode:\n1) Player vs Player\n2) Player vs Bot (unbeatable)\nSelect 1 or 2: ");
        if (scanf("%d", &mode) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        if (mode == 1 || mode == 2) break;
        printf("Please pick 1 or 2.\n");
    }

    int currentPlayer = 1; // 1 -> player X, 2 -> player O (or bot)
    int result = -1;

    // If PvBot, ask whether human wants to be X or O
    int humanIsX = 1;
    if (mode == 2) {
        int choice = 0;
        while (1) {
            printf("Do you want to play as X (goes first) or O? Enter 1 for X, 2 for O: ");
            if (scanf("%d", &choice) != 1) {
                while (getchar() != '\n');
                printf("Invalid input.\n");
                continue;
            }
            if (choice == 1) { humanIsX = 1; break; }
            if (choice == 2) { humanIsX = 0; break; }
            printf("Please enter 1 or 2.\n");
        }
        currentPlayer = 1; // X always assigned to player 1 symbolically
    }

    printBoard(board);

    while (1) {
        if (mode == 1) {
            // PvP
            char playerChar = (currentPlayer == 1) ? 'X' : 'O';
            int move = getPlayerMove(board, playerChar);
            board[move] = playerChar;
            printBoard(board);
            int check = checkWinner(board);
            if (check != -1) {
                if (check == 0) printf("Result: Draw!\n");
                else if (check == 1) printf("Result: Player X wins!\n");
                else if (check == 2) printf("Result: Player O wins!\n");
                break;
            }
            currentPlayer = 3 - currentPlayer; // switch 1<->2
        } else {
            // PvBot
            if ((humanIsX && currentPlayer == 1) || (!humanIsX && currentPlayer == 2)) {
                // Human turn
                char playerChar = (currentPlayer == 1) ? 'X' : 'O';
                int move = getPlayerMove(board, playerChar);
                board[move] = playerChar;
                printBoard(board);
            } else {
                // Bot turn
                printf("Bot is thinking...\n");
                int botMove = findBestMove(board);
                if (botMove == -1) {
                    // no move left - shouldn't generally happen here
                    printf("No moves left for bot.\n");
                } else {
                    board[botMove] = (humanIsX ? BOT : BOT); // bot symbol is 'O'
                    printf("Bot plays at position %d\n", botMove + 1);
                    printBoard(board);
                }
            }

            int check = checkWinner(board);
            if (check != -1) {
                if (check == 0) printf("Result: Draw!\n");
                else if (check == 1) {
                    // Human X won if HUMAN is 'X'
                    if (HUMAN == 'X')
                        printf("Result: Human (X) wins!\n");
                    else
                        printf("Result: Bot wins!\n");
                }
                else if (check == 2) {
                    // BOT wins
                    if (BOT == 'O')
                        printf("Result: Bot (O) wins!\n");
                    else
                        printf("Result: Human wins!\n");
                }
                break;
            }
            currentPlayer = 3 - currentPlayer; // switch
        }
    }

    printf("Game over. Thanks for playing!\n");
    return 0;
}
