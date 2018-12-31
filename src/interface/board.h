//
// Created by liu on 18-10-13.
//
__attribute__((unused)) typedef int make_iso_compilers_happy_here;

#ifndef GOMOKU_BOARD_H
#define GOMOKU_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_SIZE 15

/**
 * Indicate a point.
 */
typedef struct {
  int x, y;
} pos_t;

/**
 * Add two pos together.
 * @param pos1 The first pos.
 * @param pos2 The second pos.
 * @return The added pos.
 */
pos_t pos_add(pos_t pos1, pos_t pos2);

/**
 * Subtract two pos.
 * @param pos1 The minuend pos.
 * @param pos2 The subtrahend pos.
 * @return The subtracted pos.
 */
pos_t pos_sub(pos_t pos1, pos_t pos2);

/**
 * Detect whether the given two pos are equal.
 * @param pos1 The first pos.
 * @param pos2 The second pos.
 * @return 0 if equal, otherwise 1.
 */
int pos_cmp(pos_t pos1, pos_t pos2);

/**
 * Get the chess in the given position.
 * @param pos the position to get;
 * @return 0 if no chess in this position, 1 is black, 2 is white, -1 if invalid pos.
 */
int get_chess(pos_t pos);

int get_chess_helper(pos_t pos, void *param);

/**
 * Get the current player to set chess.
 * @return 1 is black, 2 is white.
 */
int get_player(void);

/**
 * Get the last pos chess putted.
 * @return The last pos.
 */
pos_t get_last_pos(void);

/**
 * initialize the board.
 */
void init_board(void);

/**
 * Put a chess in the given position.
 * @param pos the position to set chess
 * @return 0 if put succeed, 1 if black win, 2 if white win, -1 if is a forbidden place, -2 if it is invalid.
 */
int put_chess(pos_t pos);

/**
 * Skip the current player.
 */
void skip(void);

/**
 * Undo putting the last chess.
 * @return 0 if failed, otherwise 1.
 */
int undo(void);

#ifdef __cplusplus
}
#endif

#endif //GOMOKU_BOARD_H
