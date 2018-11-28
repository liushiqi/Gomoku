//
// Created by liu on 18-10-13.
//

#include <utils/logger.h>
#include <utils/thread.h>
#include <pthread.h>
#include <string.h>
#include "board.h"
#include "detector.h"

static int board[BOARD_SIZE][BOARD_SIZE];
static pthread_rwlock_t boardMutex;
static int currentPlayer;
static Pos historyStack[BOARD_SIZE * BOARD_SIZE + 10];
static int stackTop;

static const char *blackString = "black";
static const char *whiteString = "white";
static const char *empty = "empty";

/**
 * Detect a pos is valid or not.
 * @param pos The pos to detect.
 * @return 1 if invalid, 0 if valid.
 */
int isInvalidPos(Pos pos);

const char *playerToStr(int player);

int getChessHelper(Pos pos, void *param);

Pos posAdd(Pos pos1, Pos pos2) {
  Pos result = {pos1.x + pos2.x, pos1.y + pos2.y};
  return result;
}

Pos posSubtract(Pos pos1, Pos pos2) {
  Pos result = {pos1.x - pos2.x, pos1.y - pos2.y};
  return result;
}

int posCmp(Pos pos1, Pos pos2) {
  if (pos1.x == pos2.x && pos1.y == pos2.y)
    return 0;
  else
    return 1;
}

int getChess(Pos pos) {
  pthread_rwlock_rdlock(&boardMutex);
  int ret = board[pos.x][pos.y] + 1;
  pthread_rwlock_unlock(&boardMutex);
  return ret;
}

int getChessHelper(Pos pos, void *param) {
  pthread_rwlock_rdlock(&boardMutex);
  int ret = board[pos.x][pos.y] + 1;
  pthread_rwlock_unlock(&boardMutex);
  return ret;
}

int getPlayer() { return currentPlayer + 1; }

Pos getLastPos() { return historyStack[stackTop]; }

void initBoard() {
  memset(board, -1, sizeof(board));
  currentPlayer = 0;
  stackTop = -1;
  pthread_rwlock_init(&boardMutex, NULL);
  info("Board initialized.");
}

int putChess(Pos pos) {
  if (isInvalidPos(pos)) {
    warn("A invalid value is given, which is (%d, %d).", pos.x, pos.y)
    return -2;
  }
  if (board[pos.x][pos.y] != -1) {
    warn("A %s chess wants to put at (%d, %d), but a %s chess is already there.",
         playerToStr(currentPlayer), pos.x, pos.y, playerToStr(board[pos.x][pos.y]))
    return -2;
  } else {
    board[pos.x][pos.y] = currentPlayer;
    currentPlayer = !currentPlayer;
    historyStack[++stackTop] = pos;
    debug("A %s chess is put at position (%d, %d)", playerToStr(currentPlayer), pos.x, pos.y)
    return isWin(getChessHelper, NULL);
  }
}

int isInvalidPos(Pos pos) {
  return pos.x < 0 || pos.x > BOARD_SIZE || pos.y < 0 || pos.y > BOARD_SIZE;
}

void skip() {
  currentPlayer = !currentPlayer;
  debug("Player %s skipped his term.", playerToStr(currentPlayer))
}

int undo() {
  if (stackTop == -1) {
    error(
        "Could not revert the last step! already at the oldest step.")
    return 0;
  }
  debug("Chess reverted at pos (%d, %d)", historyStack[stackTop].x,
        historyStack[stackTop].y);
  board[historyStack[stackTop].x][historyStack[stackTop].y] = -1;
  currentPlayer = !currentPlayer;
  --stackTop;
  return 1;
}

const char *playerToStr(int player) {
  if (player == -1)
    return empty;
  else if (player == 1)
    return whiteString;
  else if (player == 0)
    return blackString;
  else {
    error("Pass wrong number: %d to playerToStr function.", player)
    return NULL;
  }
}
