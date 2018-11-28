//
// Created by liu on 18-10-13.
//

typedef int make_iso_compilers_happy_here;
extern make_iso_compilers_happy_here makeIsoCompilersHappyHere;
#ifndef GOMOKU_BOARD_H
#define GOMOKU_BOARD_H

#define BOARD_SIZE 15

/**
 * Indicate a point.
 */
typedef struct PosStructure {
  int x, y;
} Pos;

/**
 * Add two pos together.
 * @param pos1 The first pos.
 * @param pos2 The second pos.
 * @return The added pos.
 */
Pos posAdd(Pos pos1, Pos pos2);

/**
 * Subtract two pos.
 * @param pos1 The minuend pos.
 * @param pos2 The subtrahend pos.
 * @return The subtracted pos.
 */
Pos posSubtract(Pos pos1, Pos pos2);

/**
 * Detect whether the given two pos are equal.
 * @param pos1 The first pos.
 * @param pos2 The second pos.
 * @return 0 if equal, otherwise 1.
 */
int posCmp(Pos pos1, Pos pos2);

/**
 * Get the chess in the given position.
 * @param pos the position to get;
 * @return 0 if no chess in this position, 1 is black, 2 is white.
 */
int getChess(Pos pos);

/**
 * Get the current player to set chess.
 * @return 1 is black, 2 is white.
 */
int getPlayer(void);

/**
 * Get the last pos chess putted.
 * @return The last pos.
 */
Pos getLastPos(void);

/**
 * initialize the board.
 */
void initBoard(void);

/**
 * Put a chess in the given position.
 * @param pos the position to set chess
 * @return 0 if put succeed, 1 if black win, 2 if white win, -1 if is a forbidden place, -2 if it is invalid.
 */
int putChess(Pos pos);

/**
 * Skip the current player.
 */
void skip(void);

/**
 * Undo putting the last chess.
 * @return 0 if failed, otherwise 1.
 */
int undo(void);

#endif //GOMOKU_BOARD_H
