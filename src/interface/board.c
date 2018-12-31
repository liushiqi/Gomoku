//
// Created by liu on 18-10-13.
//

#include <utils/logger.h>
#include <string.h>
#include <pthread.h>
#include "board.h"
#include "detector.h"

static int search_board[BOARD_SIZE][BOARD_SIZE];
static pthread_rwlock_t boardMutex;
static int current_player;
static pos_t historyStack[BOARD_SIZE * BOARD_SIZE + 10];
static int stackTop;

static const wchar_t *blackString = L"black";
static const wchar_t *whiteString = L"white";
static const wchar_t *empty = L"empty";

/**
 * Detect a pos is valid or not.
 * @param pos The pos to detect.
 * @return 1 if invalid, 0 if valid.
 */
int is_invalid_pos(pos_t pos);

const wchar_t *player_to_str(int player);

int get_chess_helper(pos_t pos, void *param);

pos_t pos_add(pos_t pos1, pos_t pos2) {
  pos_t result = {pos1.x + pos2.x, pos1.y + pos2.y};
  return result;
}

pos_t pos_sub(pos_t pos1, pos_t pos2) {
  pos_t result = {pos1.x - pos2.x, pos1.y - pos2.y};
  return result;
}

int pos_cmp(pos_t pos1, pos_t pos2) {
  if (pos1.x == pos2.x && pos1.y == pos2.y)
    return 0;
  else
    return 1;
}

int get_chess(pos_t pos) {
  if (is_invalid_pos(pos))
    return -1;
  pthread_rwlock_rdlock(&boardMutex);
  int ret = search_board[pos.x][pos.y] + 1;
  pthread_rwlock_unlock(&boardMutex);
  return ret;
}

int get_chess_helper(pos_t pos, __attribute__((unused)) void *param) {
  if (is_invalid_pos(pos))
    return -1;
  pthread_rwlock_rdlock(&boardMutex);
  int ret = search_board[pos.x][pos.y] + 1;
  pthread_rwlock_unlock(&boardMutex);
  return ret;
}

int get_player() { return current_player + 1; }

pos_t get_last_pos() { return historyStack[stackTop]; }

void init_board() {
  memset(search_board, -1, sizeof(search_board));
  current_player = 0;
  stackTop = -1;
  pthread_rwlock_init(&boardMutex, NULL);
  INFO(L"Board initialized.");
}

int put_chess(pos_t pos) {
  if (is_invalid_pos(pos)) {
    WARN(L"A invalid value is given, which is (%d, %d).", pos.x, pos.y);
    return -2;
  } else if (is_forbidden(pos, current_player + 1, get_chess_helper, NULL, 0)) {
    WARN(L"A forbidden place is given, which is (%d, %d).", pos.x, pos.y);
    return -1;
  } else if (search_board[pos.x][pos.y] != -1) {
    WARN(L"A %s chess wants to put at (%d, %d), but a %s chess is already there.",
         player_to_str(current_player), pos.x, pos.y, player_to_str(search_board[pos.x][pos.y]));
    return -2;
  } else {
    search_board[pos.x][pos.y] = current_player;
    DEBUG(L"A %ls chess is put at position (%d, %d)", player_to_str(current_player), pos.x, pos.y);
    current_player = !current_player;
    historyStack[++stackTop] = pos;
    int i = is_win(historyStack[stackTop], get_chess_helper, NULL);
    return i;
  }
}

int is_invalid_pos(pos_t pos) {
  return pos.x < 0 || pos.x > BOARD_SIZE || pos.y < 0 || pos.y > BOARD_SIZE;
}

void skip() {
  current_player = !current_player;
  DEBUG(L"Player %ls skipped his term.", player_to_str(current_player));
}

int undo() {
  if (stackTop == -1) {
    ERROR(L"Could not revert the last step! already at the oldest step.");
    return 0;
  }
  DEBUG(L"Chess reverted at pos (%d, %d)", historyStack[stackTop].x, historyStack[stackTop].y);
  search_board[historyStack[stackTop].x][historyStack[stackTop].y] = -1;
  current_player = !current_player;
  --stackTop;
  return 1;
}

const wchar_t *player_to_str(int player) {
  if (player == -1)
    return empty;
  else if (player == 1)
    return whiteString;
  else if (player == 0)
    return blackString;
  else {
    ERROR(L"Pass wrong number: %d to player_to_str function.", player);
    return NULL;
  }
}
