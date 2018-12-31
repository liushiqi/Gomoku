//
// Created by liu on 18-10-20.
//

#ifndef GOMOKU_VALUES_H
#define GOMOKU_VALUES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <wchar.h>

// Request
extern const wchar_t *const REQUEST_PLAYER_NUMBER;

extern const wchar_t *const REQUEST_CHESS_TYPE;

extern const wchar_t *const REQUEST_NUMBER;

// Board
extern const wchar_t *const BOARD_HEADER_LINES;

extern const wchar_t *const BOARD_FOOT_LINES;

extern const wchar_t *const INPUT_DIRECTION;

// player help
extern const wchar_t *const BLACK_TERM_MESSAGE;

extern const wchar_t *const BLACK_WIN_MESSAGE;

extern const wchar_t *const WHITE_TERM_MESSAGE;

extern const wchar_t *const WHITE_WIN_MESSAGE;

extern const wchar_t *const QUIT_MESSAGE;

// error
extern const wchar_t *const WRONG_INPUT_ERROR;

extern const wchar_t *const FORBIDDEN_POS_ERROR;

extern const wchar_t *const OLDEST_STATE_WARNING;

// chess
extern const char *RED_COLOR;
extern const char *DEFAULT_COLOR;
extern const wchar_t BLACK_CHESS;
extern const wchar_t WHITE_CHESS;
extern const wchar_t LEFT_TOP;
extern const wchar_t RIGHT_TOP;
extern const wchar_t LEFT_GROUND;
extern const wchar_t RIGHT_GROUND;
extern const wchar_t TOP_EDGE;
extern const wchar_t GROUND_EDGE;
extern const wchar_t LEFT_EDGE;
extern const wchar_t RIGHT_EDGE;
extern const wchar_t MIDDLE;

#ifdef __cplusplus
}
#endif

#endif //GOMOKU_VALUES_H
