//
// Created by liu on 18-10-17.
//

#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <utils/values.h>
#include <utils/signals.h>
#include <utils/logger.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ai/ai.h>
#include "screen.h"

/** 0 if is single player, 1 if is multi player. */
static int game_type;

/** 1 is black, 2 is white. */
static int player;

/**
 * Request a int for the given request from standard input.
 * @param request The request string.
 * @param value The value to receive.
 * @return 1 if read successfully, 0 if eof met.
 */
int request_int(const wchar_t *request, int *value);

/**
 * Request a wide string for the given request from standard input.
 * @param request The request string.
 * @param value The value to receive.
 * @param count The max length of a line.
 * @return 1 if read successfully, 0 if eof met.
 */
int request_line(const wchar_t *request, wchar_t *value, int count);

/**
 * Get the input from either standard input or the AI calc result and put a chess.
 * @return 0 if succeed, 1 if black win, 2 if white win, -1 if interrupted, -2 if fatal error.
 */
int do_user_input(void);

void init_screen(void) {
  clear();
  wprintf(REQUEST_PLAYER_NUMBER);
  while (request_int(REQUEST_NUMBER, &game_type) && game_type != 1 && game_type != 2) {
    WARN(L"Wrong input: %d", game_type);
    wprintf(WRONG_INPUT_ERROR);
  }
  if (feof(stdin)) {
    WARN(L"Exited unexpectedly.");
    exit(0);
  }
  --game_type;
  INFO(L"Game mode is %ls player.", game_type ? L"multi" : L"single");
  if (!game_type) {
    wprintf(REQUEST_CHESS_TYPE);
    while (request_int(REQUEST_NUMBER, &player) && (player != 1 && player != 2)) {
      WARN(L"Wrong input %d", game_type);
      wprintf(WRONG_INPUT_ERROR);
    }
    if (feof(stdin)) {
      WARN(L"Exited unexpectedly.");
      exit(0);
    }
    INFO(L"player chose %s.", player - 1 ? "white" : "black");
    init_ai(player == 1 ? 2 : 1);
  }
  INFO(L"Screen initialized.");
}

void refresh() {
  clear();
  wprintf(BOARD_HEADER_LINES);
  for (int i = BOARD_SIZE - 1; i >= 0; --i) {
    wprintf(L"%02d  ", i + 1);
    for (int j = 0; j < BOARD_SIZE; ++j) {
      pos_t pos = {i, j};
      switch (get_chess(pos)) {
        case 1:
          wprintf(L"%s%lc%s",
                  pos_cmp(pos, get_last_pos()) ? "" : RED_COLOR,
                  BLACK_CHESS,
                  pos_cmp(pos, get_last_pos()) ? "" : DEFAULT_COLOR);
          break;
        case 2:
          wprintf(L"%s%lc%s",
                  pos_cmp(pos, get_last_pos()) ? "" : RED_COLOR,
                  WHITE_CHESS,
                  pos_cmp(pos, get_last_pos()) ? "" : DEFAULT_COLOR);
          break;
        case 0:
          if (pos.x == BOARD_SIZE - 1)
            putwchar(pos.y == 0 ? LEFT_TOP : pos.y == BOARD_SIZE - 1 ? RIGHT_TOP : TOP_EDGE);
          else if (pos.x == 0)
            putwchar(pos.y == 0 ? LEFT_GROUND : pos.y == BOARD_SIZE - 1 ? RIGHT_GROUND : GROUND_EDGE);
          else
            putwchar(pos.y == 0 ? LEFT_EDGE : pos.y == BOARD_SIZE - 1 ? RIGHT_EDGE : MIDDLE);
          break;
        default: ERROR(L"This case should not happen!!!! the value of chess is %d", get_chess(pos));
          break;
      }
    }
    putwchar('\n');
  }
  wprintf(BOARD_FOOT_LINES);
  pos_t last = get_last_pos();
  if (last.x == -1) { /* EMPTY */ }
  else wprintf(L"%ls %c%d\n", LAST_POS_MESSAGE, last.y + 'A', last.x + 1);
}

void *loop(__attribute__((unused)) void *ptr) {
  sigset_t mask;
  sigaddset(&mask, SIGINT);
  pthread_sigmask(SIG_BLOCK, &mask, NULL);
  while (1) {
    int result;
    refresh();
    result = do_user_input();
    switch (result) {
      case 0:continue;
      case 1: INFO(L"Black win!!!");
        refresh();
        wprintf(BLACK_WIN_MESSAGE);
        return NULL;
      case 2: INFO(L"White win!!!");
        refresh();
        wprintf(WHITE_WIN_MESSAGE);
        return NULL;
      case -1: INFO(L"Program quited with no one wins.");
        wprintf(QUIT_MESSAGE);
        return NULL;
      default: ERROR(L"This should not happen.");
        return NULL;
    }
  }
}

void clear() {
  wprintf(L"\033c");
}

int do_user_input() {
  wchar_t line[1000];
  pos_t pos = {-1, -1};
  if (game_type || get_player() == player) {
    int error_code;
    wprintf(INPUT_DIRECTION);
    while ((error_code = request_line(get_player() - 1 ? WHITE_TERM_MESSAGE : BLACK_TERM_MESSAGE, line, 1000)) > -1) {
      if (error_code == 0) {
        WARN(L"Exited unexpectedly.");
        return -1;
      }
      TRACE(L"Input string is %ls", line);
      if (!wcscmp(line, L"quit")) return -1;
      else if (!wcscmp(line, L"undo")) {
        if (undo()) return 0;
        else wprintf(OLDEST_STATE_WARNING);
        continue;
      } else if (!wcscmp(line, L"skip")) {
        skip();
        return 0;
      } else if (iswalpha((wint_t) line[0])) {
        wchar_t *ptr;
        pos.y = (wchar_t) towlower((wint_t) line[0]) - 'a';
        pos.x = (int) wcstol(line + 1, &ptr, 10) - 1;
        if (ptr == line + 1) {
          WARN(L"Wrong input is %ls", line);
          wprintf(WRONG_INPUT_ERROR);
          continue;
        }
      } else {
        WARN(L"Wrong input %ls", line);
        wprintf(WRONG_INPUT_ERROR);
        continue;
      }
      switch (put_chess(pos)) {
        case 0:return 0;
        case 1:return 1;
        case 2:return 2;
        case -1:wprintf(FORBIDDEN_POS_ERROR);
          continue;
        case -2:wprintf(WRONG_INPUT_ERROR);
          continue;
        default: ERROR(L"This should not happen!");
          return -2;
      }
    }
    return -2;
  } else {
    switch (put_chess(get_ai_input())) {
      case 0:return 0;
      case 1: return 1;
      case 2:return 2;
      default:ERROR(L"This should not happen!");
        return -2;
    }
  }
}

int request_int(const wchar_t *request, int *value) {
  wchar_t line[1000];
  while (request_line(request, line, 1000) && !iswdigit((wint_t) line[0])) {
    WARN(L"Wrong input %ls", line);
    wprintf(WRONG_INPUT_ERROR);
  }
  if (feof(stdin))
    return 0;
  wchar_t *pos;
  *value = (int) wcstol(line, &pos, 10);
  if (*pos == '\0') return 0;
  return 1;
}

int request_line(const wchar_t *request, wchar_t *value, int count) {
  wprintf(request);
  if (fgetws(value, count, stdin) == NULL) {
    if (get_sigint_status()) {
      set_sigint_status(0);
      wprintf(L"\n");
      return request_line(request, value, count);
    } else if (feof(stdin))
      return 0;
  } else {
    if (value[wcslen(value) - 1] == '\n')
      value[wcslen(value) - 1] = 0;
    return 1;
  }
  return 0;
}