//
// Created by liu on 18-10-13.
//

#include <string.h>
#include "board.h"
#include "../utils/logger.h"

int board[BOARD_SIZE][BOARD_SIZE];
int currentPlayer;
Pos historyStack[BOARD_SIZE * BOARD_SIZE + 10];
int stackTop;

const char *blackString = "black";
const char *whiteString = "white";
const char *empty = "empty";

const char *playerToStr(int player) {
    if (player == -1) return empty;
    else if (player == 1) return whiteString;
    else if (player == 0) return blackString;
    else {
        error("Pass wrong number: %d to playerToStr function.", player)
        return NULL;
    }
}

int posCmp(Pos pos1, Pos pos2) {
    if (pos1.x == pos2.x && pos1.y == pos2.y) return 0;
    else return 1;
}

int getChess(Pos pos) {
    return board[pos.x][pos.y] + 1;
}

int getPlayer() {
    return currentPlayer + 1;
}

Pos getLastPos() {
    return historyStack[stackTop];
}

void initBoard() {
    memset(board, -1, sizeof(board));
    currentPlayer = 0;
    stackTop = -1;
    info("Board initialized.");
}

int putChess(Pos pos) {
    if (pos.x < 0 || pos.x > BOARD_SIZE || pos.y < 0 || pos.y > BOARD_SIZE) {
        warn("A invalid value is given, which is (%d, %d).", pos.x, pos.y)
        return -2;
    }
    if (board[pos.x][pos.y] != -1) {
        warn("A %s chess wants to put at (%d, %d), but a %s chess is already there.", playerToStr(currentPlayer), pos.x,
             pos.y, playerToStr(board[pos.x][pos.y]))
        return -2;
    } else {
        board[pos.x][pos.y] = currentPlayer;
        currentPlayer = !currentPlayer;
        historyStack[++stackTop] = pos;
        debug("A %s chess is put at position (%d, %d)", playerToStr(currentPlayer), pos.x, pos.y)
        return 0;
    }
}

void skip() {
    currentPlayer = !currentPlayer;
    debug("Player %s skipped his term.", playerToStr(currentPlayer))
}

int undo() {
    if (stackTop == -1) {
        error("Could not revert the last step! already at the oldest step.")
        return 0;
    }
    debug("Chess reverted at pos (%d, %d)", historyStack[stackTop].x, historyStack[stackTop].y);
    currentPlayer = !currentPlayer;
    --stackTop;
    return 1;
}
