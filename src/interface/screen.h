//
// Created by liu on 18-10-17.
//

#ifndef GOMOKU_SCREEN_H
#define GOMOKU_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <wchar.h>
#include "board.h"

/**
 * Initialize the screen and request for game details.
 */
void init_screen(void);

/**
 * Clear the contents on the screen.
 */
void clear(void);

/**
 * The game loop
 * @return NULL.
 */
void *loop(void *);

/**
 * Reprint the screen.
 */
void refresh(void);

#ifdef __cplusplus
}
#endif

#endif //GOMOKU_SCREEN_H
