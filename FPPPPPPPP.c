#include <stdio.h>
#include <stdbool.h>

char board[3][3];

void printPanduan() {
    printf("Panduan posisi (1–9):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %d ", i * 3 + j + 1);
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) printf("---+---+---\n");
    }
    printf("\n");
}

void printBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) printf("---+---+---\n");
    }
    printf("\n");
}

bool Winner(char player) {
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}

bool Draw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') return false;
        }
    }
    return true;
}

void getCoordinates(int move, int *row, int *col) {
    *row = (move - 1) / 3;
    *col = (move - 1) % 3;
}

void playerMove(int playerNum, char symbol) {
    int move, row, col;
    while (1) {
        printf("Pemain %d (%c), pilih petak (1–9): ", playerNum, symbol);
        scanf("%d", &move);
        if (move < 1 || move > 9) {
            printf("hanya pilih 1-9.\n");
            continue;
        }
        getCoordinates(move, &row, &col);
        if (board[row][col] != ' ') {
            printf("Petak penuh pilih yang lain.\n");
            continue;
        }
        board[row][col] = symbol;
        break;
    }
}

int evaluate() {
    if (Winner('O')) return 10;
    if (Winner('X')) return -10;
    return 0;
}

int minimax(int depth, bool isMaximizing) {
    int score = evaluate();
    if (score == 10 || score == -10) return score;
    if (Draw()) return 0;

    if (isMaximizing) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    int val = minimax(depth + 1, false);
                    if (val > best) best = val;
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    int val = minimax(depth + 1, true);
                    if (val < best) best = val;
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

void botMove() {
    int bestVal = -1000;
    int bestRow = -1, bestCol = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                int moveVal = minimax(0, false);
                board[i][j] = ' ';
                if (moveVal > bestVal) {
                    bestRow = i;
                    bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    board[bestRow][bestCol] = 'O';
    printf("Bot milih petak %d\n", bestRow * 3 + bestCol + 1);
}

int main() {
    int mode;
    printf("=== TIC TAC TOE ===\n");
    printf("1. Pemain vs Pemain\n");
    printf("2. Pemain vs Bot\n");
    printf("Pilih mode: ");
    scanf("%d", &mode);

    printPanduan();

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';

    int currentPlayer = 1;
    char currentSymbol = 'X';
    bool gameOver = false;

    while (!gameOver) {
        printBoard();
        if (mode == 1 || currentPlayer == 1) {
            playerMove(currentPlayer, currentSymbol);
        } else {
            botMove();
        }

        if (Winner(currentSymbol)) {
            printBoard();
            if (mode == 1)
                printf("Pemain %d menang!\n", currentPlayer);
            else
                printf(currentPlayer == 1 ? "Kamu menang!\n" : "Bot menang!\n");
            gameOver = true;
        } else if (Draw()) {
            printBoard();
            printf("Hasil seri!\n");
            gameOver = true;
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        currentSymbol = (currentSymbol == 'X') ? 'O' : 'X';
    }

    printf("Game selesai!\n");
    return 0;
}