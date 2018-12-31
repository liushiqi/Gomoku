//
// Created by liu on 18-11-10.
//

#ifndef GOMOKU_DETECTOR_H
#define GOMOKU_DETECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

/**
 * Get the max number of living chess in the given direction.
 * @param point The point to check.
 * @param direction The direction to check.
 * @param get_chess_func The function applied to get the chess of a point
 * @param param The params passed to get_chess_func.
 * @return 1 - XO#OX<br>
 *         2 - XO#LOX or XO#OLOX<br>
 *         3 - XO###OX or AO##O#A<br>
 *         4 - AO####X or A###O#A<br>
 *         5 - #####<br>
 *         6 - six or more - ######....<br>
 *         0 - others<br>
 *         where X is any non-black chess or the place black couldn't place, O is the place black could place, A is O and X, # is black chess, L is where black last place.<br>
 */
int search_chess_one_dir(pos_t point, pos_t direction, int (*get_chess_func)(pos_t, void *), void *param, int depth);

/**
 * Get the message about whether someone wins now.
 * @param pos The pos to test.
 * @param get_chess_func The function applied to get the chess of a point.
 * @param param The params passed to get_chess_func.
 * @return 0 if nobody win, 1 if black win, 2 if white win, -1 if forbidden.
 */
int is_win(pos_t pos, int (*get_chess_func)(pos_t, void *), void *param);

/**
 * Whether the chess put in the given pos is forbidden.
 * @param pos The pos to test.
 * @param get_chess_func The function applied to get the chess of a point.
 * @param param The params passed to get_chess_func.
 * @return 0 if not forbidden, 1 otherwise.
 */
int is_forbidden(pos_t pos, int player, int (*get_chess_func)(pos_t, void *), void *param, int depth);

#ifdef __cplusplus
}
#endif

#endif //GOMOKU_DETECTOR_H
