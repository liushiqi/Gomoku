//
// Created by liu on 18-9-7.
//

#ifndef CCLASS_INTERFACE_H
#define CCLASS_INTERFACE_H

#include <string.h>

/**
 * The chess on the board, -1 is no chess, 0 is black, 1 is white.
 */
extern int board[21][21];

/**
 * The mode of playing, 0 is single player, 1 is multi player.
 */
extern int mode;

/**
 * the player current should put chess, 0 is black, 1 is white.
 */
extern int player;

/**
 * the number of player if is single mode.
 */
extern int human;

/**
 * The process of initializing the board, will require for the type of playing too.
 */
void initBoard();

/**
 * Get the player who wins.
 * @return 1 if black wins, 2 if white wins, 0 if no one wins.
 */
int winner();

/**
 * Display the board.
 */
void displayBoard();

/**
 * print a message requesting for input.
 */
void requestInput();

/**
 * Put a chess in the given place.
 * @param x the x axis to store.
 * @param y thy y axis to store.
 * @return 0 if succeed, 1 if the target pos have had an chess, TODO 2 if it is a forbidden place.
 */
int setChess(int x, int y);

#endif //CCLASS_INTERFACE_H
