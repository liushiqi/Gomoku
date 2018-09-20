//
// Created by liu on 18-9-7.
//

#include <stdbool.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stddef.h>
#include <wchar.h>

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

const wchar_t *header = L"=================五子棋=================\n\n\n\n";

const wchar_t *request = L"=================五子棋=================\n\n\n\n"
                         L"欢迎使用五子棋交互程序，请选择玩家数量。\n"
                         L"1：单人                    2：多人\n"
                         L"请输入编号：";

const wchar_t *request2 = L"清选择您要使用的棋子颜色。\n"
                          L"1：黑色         2：白色\n"
                          L"请输入编号：";

const wchar_t *direct = L"使用数字+字母的形式输入，如1A或1a，-1q以退出。\n";

const wchar_t *blackTerm = L"请黑棋下子：";

const wchar_t *whiteTerm = L"请白棋下子：";

const wchar_t *rows = L"     A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S\n";

#ifdef WIN32
const wchar_t *top = L"   ┏━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┯━┓\n";

const wchar_t *middle = L"   ┠─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┨\n";

const wchar_t *ground = L"   ┗━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┷━┛\n\n\n\n";

const wchar_t *nothing = L"  ";

const wchar_t *chessBlack = L"●";

const wchar_t *chessWhite = L"○";
#else
const wchar_t *top = L"   ┏━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┓\n";

const wchar_t *middle = L"   ┠───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┨\n";

const wchar_t *ground = L"   ┗━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┛\n\n\n\n";

const wchar_t *nothing = L"   ";

const wchar_t *chessBlack = L" ● ";

const wchar_t *lastChessBlack = L" ◉ ";

const wchar_t *chessWhite = L" ○ ";

const wchar_t *lastChessWhite = L" ◎ ";
#endif

const wchar_t thick = 0x2503;

const wchar_t thin = 0x2502;

int board[21][21];
int mode;
int player;
int human;
int dirs[4][2] = {{1,  0},
                  {1,  1},
                  {0,  1},
                  {-1, 1}};
int lastX, lastY;

void clearScreen() {
#ifdef WIN32
    system("cls");
#else
    wprintf(L"\033c");
#endif
}

void initBoard() {
    setlocale(LC_ALL, "");
    memset(board, -1, sizeof(board));
    clearScreen();
    wprintf(request);
    wscanf(L"%d", &mode);
    mode -= 1;
    if (!mode) {
        wprintf(request2);
        wscanf(L"%d", &human);
        human -= 1;
    }
    player = 0;
}

bool setChess(int x, int y) {
    if (x < 1 || x > 19 || y < 1 || y > 19) return false;
    if (board[x][y] != -1) return false;
    else {
        lastX = x;
        lastY = y;
        board[x][y] = player;
        player = !player;
        return true;
    }
}

void requestInput() {
    wprintf(direct);
    if (player) wprintf(whiteTerm);
    else wprintf(blackTerm);
}

void displayBoard() {
    clearScreen();
    wprintf(header);
    wprintf(rows);
    for (int i = 1; i < 20; ++i) {
        if (i == 1) wprintf(top);
        else wprintf(middle);
        wprintf(L"%02d %lc", i, thick);
        for (int j = 1; j < 20; ++j) {
            wprintf(L"%ls%lc",
                    board[i][j] == -1 ? nothing : board[i][j] == 0 ? ((lastX == i && lastY == j) ? lastChessBlack
                                                                                                 : chessBlack)
                                                                   : ((lastX == i && lastY == j) ? lastChessWhite
                                                                                                 : chessWhite),
                    j == 19 ? thick : thin);
        }
        wprintf(L"\n");
    }
    wprintf(ground);
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
            if (board[i][j] != -1 && valid(i, j)) return board[i][j];
        }
    }
    return 0;
}
