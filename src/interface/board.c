//
// Created by liu on 18-10-13.
//

#include <utils/logger.h>
#include "board.h"

int board[BOARD_SIZE][BOARD_SIZE];
int currentPlayer;
Pos historyStack[BOARD_SIZE * BOARD_SIZE + 10];
int stackTop;

Pos directions[] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1},
                    {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

const char *blackString = "black";
const char *whiteString = "white";
const char *empty = "empty";

/**
 * Detect a pos is valid or not.
 * @param pos The pos to detect.
 * @return 1 if invalid, 0 if valid.
 */
int isInvalidPos(Pos pos);

/**
 * Get the max number of living chess in the given direction.
 * @param point The point to check.
 * @param direction The direction to check.
 * @return 1 if like XO@OX
 *         2 if like XO@@OX
 *         3X if like XO@@@OX or XO@@O@X, 30 is XO@L@OX, 31 is XOL@@OX.
 *         4X if like XO@@@@X or X@@@O@X or X@@O@@X
 *             40 if forward equal backward, 41 otherwise.
 *         5 if @@@@@, six or more if @@@@@@...,
 *         where X is any non-black chess, O is the place black could place, @
 * is black chess, L is where black last place.
 */
int searchChessInOneDirection(Pos point, Pos direction,
                              int (*getChessFunc)(Pos));

const char *playerToStr(int player);

/**
 * Get the message about whether someone wins now.
 * @return 0 if nobody win, 1 if black win, 2 if white win, -1 if forbidden.
 */
int isWin();

/**
 * Whether the last chess is be put at forbidden place.
 * @return 0 if not forbidden, 1 otherwise.
 */
int isForbidden(Pos pos);

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

int getChess(Pos pos) { return board[pos.x][pos.y] + 1; }

int getPlayer() { return currentPlayer + 1; }

Pos getLastPos() { return historyStack[stackTop]; }

void initBoard() {
  memset(board, -1, sizeof(board));
  currentPlayer = 0;
  stackTop = -1;
  info("Board initialized.");
}

int putChess(Pos pos) {
  if (isInvalidPos(pos)) {
    warn("A invalid value is given, which is (%d, %d).", pos.x,
         pos.y)
    return -2;
  }
  if (board[pos.x][pos.y] != -1) {
    warn(
        "A %s chess wants to put at (%d, %d), but a %s chess is already there.",
        playerToStr(currentPlayer), pos.x, pos.y,
        playerToStr(board[pos.x][pos.y]))
    return -2;
  } else {
    board[pos.x][pos.y] = currentPlayer;
    currentPlayer = !currentPlayer;
    historyStack[++stackTop] = pos;
    debug("A %s chess is put at position (%d, %d)", playerToStr(currentPlayer),
          pos.x, pos.y)
    return isWin();
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

int isWin() {
  int result;
  for (int i = 0; i < 4; ++i) {
    if ((result = searchChessInOneDirection(historyStack[stackTop],
                                            directions[i], getChess)) >= 5)
      return !currentPlayer && result > 5 ? -1 : currentPlayer + 1;
  }
  return 0;
}

int searchChessInOneDirection(Pos point, Pos direction,
                              int (*getChessFunc)(Pos)) {
  Pos current = posAdd(point, direction);
  int blank = 0, forwardChess = 1;
  for (int i = 0; i < 4; ++i) {
    if (isInvalidPos(current))
      break;
    else if (getChessFunc(current) == -1) {
      if (blank <= 1) {
        ++blank;
      } else
        break;
    } else if (getChessFunc(current) != currentPlayer) {
    } else {
      ++forwardChess;
    }
  }
  return 0;
}

int isForbidden(Pos pos) {
  int three = 0, four = 0, halfThree = 0, halfFour = 0;
  for (int i = 0; i < 8; ++i) {
    switch (searchChessInOneDirection(historyStack[stackTop], directions[i],
                                      getChess)) {
      case 30:++halfThree;
        break;
      case 31:++three;
        break;
      case 40:++halfFour;
        break;
      case 41:++four;
        break;
      default:break;
    }
  }
  return (three + halfThree / 2) >= 2 || (four + halfFour / 2) >= 2;
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
