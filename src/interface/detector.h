//
// Created by liu on 18-11-10.
//

#ifndef GOMOKU_DETECTOR_H
#define GOMOKU_DETECTOR_H

#include <stdarg.h>
#include "board.h"

/**
 * Get the max number of living chess in the given direction.
 * @param point The point to check.
 * @param direction The direction to check.
 * @param getChessFunc The function applied to get the chess of a point
 * @param param The params passed to getChessFunc.
 * @return 1  - XO#OX<br>
 *         2  - XO##OX<br>
 *         3X - XO###OX or XO##O#X, 30 - XO#L#OX, 31 - otherwise.<br>
 *         4X - XO####X or X###O#X or X##O##X, 40 - XO#L##X or X##LO#X, 41 - otherwise.<br>
 *         5  - #####, six or more - ######....<br>
 *         where X is any non-black chess or the place black couldn't place, O is the place black could place, # is black chess, L is where black last place.<br>
 */
int searchChessInOneDirection(Pos point, Pos direction, int (*getChessFunc)(Pos, void *), void *param);

/**
 * Get the message about whether someone wins now.
 * @return 0 if nobody win, 1 if black win, 2 if white win, -1 if forbidden.
 */
int isWin(int (*getChessFunc)(Pos, void *), void *param);

/**
 * Whether the chess put in the given pos is forbidden.
 * @param pos The pos to test.
 * @param getChessFunc The function applied to get the chess of a point
 * @return 0 if not forbidden, 1 otherwise.
 */
int isForbidden(Pos pos, int (*getChessFunc)(Pos, void *), void *param);

#endif //GOMOKU_DETECTOR_H
