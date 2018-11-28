//
// Created by liu on 18-10-17.
//

#ifndef GOMOKU_SCREEN_H
#define GOMOKU_SCREEN_H

#include <wchar.h>
#include "board.h"

/**
 * Initialize the screen and request for game details.
 */
void initScreen(void);

/**
 * Clear the contents on the screen.
 */
void clear(void);

/**
 * The game loop
 * @return 0 if succeed.
 */
int loop(void *);

/**
 * Reprint the screen.
 */
void refresh(void);

#endif //GOMOKU_SCREEN_H
