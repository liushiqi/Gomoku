//
// Created by liu on 18-9-7.
//

#include <lzma.h>
#include <stdbool.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

const wchar_t *header = L"=================五子棋=================\n\n\n\n";

const wchar_t *request = L"=================五子棋=================\n\n\n\n"
                         "欢迎使用五子棋交互程序，请选择玩家数量。\n"
                         "1：单人                    2：多人\n";

const wchar_t *rows = L"   A B C D E F G H I J K L M N O P Q R S\n";

const wchar_t *blackTerm = L"请黑棋下子：";

const wchar_t *whiteTerm = L"请白棋下子：";

const wchar_t chessBlack = 0x2b24;
const wchar_t chessWhite = 0x25ef;
const wchar_t leftTop = 0x250c;
const wchar_t rightTop = 0x2510;
const wchar_t leftGround = 0x2514;
const wchar_t rightGround = 0x2518;
const wchar_t top = 0x252c;
const wchar_t ground = 0x2534;
const wchar_t left = 0x251c;
const wchar_t right = 0x2524;
const wchar_t middle = 0x253c;
const wchar_t separate = 0x2500;

int board[21][21];
int mode;
int player;
int searched[21][21];
int dirs[4][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}};

void clearScreen() {
#ifdef WIN32
    system('cls')
#else
    printf("\033c");
#endif
}

void initBoard() {
    setlocale(LC_ALL, "");
    memset(board, 0, sizeof(board));
    clearScreen();
    printf("%ls", request);
    scanf("%d", &mode);
    player = 0;
}

bool setChess(int x, int y) {
    if (board[x][y] != 0) return false;
    else {
        board[x][y] = player;
        player = player == 1 ? 2 : 1;
        return true;
    }
}

void requestInput() {
    if (player == 1) printf("%ls", blackTerm);
    else printf("%ls", whiteTerm);
}

void displayBoard() {
    clearScreen();
    printf("%ls", header);
    printf("%ls", rows);
    for (int i = 1; i < 20; ++i) {
        printf("%02d ", i);
        for (int j = 1; j < 20; ++j) {
            if (board[i][j] == 1) printf("%lc", chessBlack);
            if (board[i][j] == 2) printf("%lc", chessWhite);
            else {
                if (i == 0 && j == 0) printf("%lc", leftTop);
                else if (i == 0 && j == 18) printf("%lc", rightTop);
                else if (i == 18 && j == 0) printf("%lc", leftGround);
                else if (i == 18 && j == 18) printf("%lc", rightGround);
                else if (i == 0) printf("%lc", top);
                else if (i == 18) printf("%lc", ground);
                else if (j == 0) printf("%lc", left);
                else if (j == 18) printf("%lc", right);
                else printf("%lc", middle);
                if (j < 18) printf("%lc", separate);
            }
        }
        printf("\n");
    }
}

int valid(int x, int y) {
    for (int i = 0; i < 4; ++i) {
        int yes = 1;
        for (int j = 1; j <= 5; ++j) {
            if (board[x + dirs[i][0] * j][y + dirs[i][1] * j] != board[x][y]) yes = 0;
        }
        if (yes) return 1;
    }
    return 0;
}

int winner() {
    for (int i = 1; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            if (valid(i, j)) return board[i][j];
        }
    }
    return 0;
}
