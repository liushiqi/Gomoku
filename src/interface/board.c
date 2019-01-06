//
// Created by liu on 18-10-13.
//

#include <utils/logger.h>
#include <string.h>
#include <pthread.h>
#include "board.h"
#include "detector.h"

static int search_board[BOARD_SIZE][BOARD_SIZE];
static pthread_rwlock_t board_mutex;
static int current_player;
static pos_t history_stack[BOARD_SIZE * BOARD_SIZE + 10];
static int stack_top;

static const wchar_t *black_string = L"black";
static const wchar_t *white_string = L"white";
static const wchar_t *empty = L"empty";

const wchar_t *player_to_str(int player);

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
  pthread_rwlock_rdlock(&board_mutex);
  int ret = search_board[pos.x][pos.y] + 1;
  pthread_rwlock_unlock(&board_mutex);
  return ret;
}

int get_chess_helper(pos_t pos, __attribute__((unused)) void *param) {
  if (is_invalid_pos(pos))
    return -1;
  pthread_rwlock_rdlock(&board_mutex);
  int ret = search_board[pos.x][pos.y] + 1;
  pthread_rwlock_unlock(&board_mutex);
  return ret;
}

int get_player() { return current_player + 1; }

pos_t temp_pos = {-1, -1};
pos_t get_last_pos() { if (stack_top == -1) return temp_pos; else return history_stack[stack_top]; }

void init_board() {
  memset(search_board, -1, sizeof(search_board));
  current_player = 0;
  stack_top = -1;
  pthread_rwlock_init(&board_mutex, NULL);
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
    WARN(L"A %ls chess wants to put at (%d, %d), but a %ls chess is already there.",
         player_to_str(current_player), pos.x, pos.y, player_to_str(search_board[pos.x][pos.y]));
    return -2;
  } else {
    DEBUG(L"A %ls chess is put at position (%d, %d)", player_to_str(current_player), pos.x, pos.y);
    pthread_rwlock_wrlock(&board_mutex);
    search_board[pos.x][pos.y] = current_player;
    pthread_rwlock_unlock(&board_mutex);
    current_player = !current_player;
    history_stack[++stack_top] = pos;
    return is_win(history_stack[stack_top], get_chess_helper, NULL);
  }
}

int is_invalid_pos(pos_t pos) {
  return pos.x < 0 || pos.x >= BOARD_SIZE || pos.y < 0 || pos.y >= BOARD_SIZE;
}

void skip() {
  DEBUG(L"Player %ls skipped his term.", player_to_str(current_player));
  current_player = !current_player;
}

int undo() {
  if (stack_top == -1) {
    ERROR(L"Could not revert the last step! already at the oldest step.");
    return 0;
  }
  DEBUG(L"Chess reverted at pos (%d, %d)", history_stack[stack_top].x, history_stack[stack_top].y);
  search_board[history_stack[stack_top].x][history_stack[stack_top].y] = -1;
  current_player = !current_player;
  --stack_top;
  return 1;
}

const wchar_t *player_to_str(int player) {
  if (player == -1)
    return empty;
  else if (player == 1)
    return white_string;
  else if (player == 0)
    return black_string;
  else {
    ERROR(L"Pass wrong number: %d to player_to_str function.", player);
    return NULL;
  }
}
