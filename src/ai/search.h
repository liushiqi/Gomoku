//
// Created by liu on 19-1-4.
//

#ifndef GOMOKU_SEARCH_H
#define GOMOKU_SEARCH_H

#include <interface/board.h>

/**
 * Init all search queues.
 * @param player the player computer use.
 */
void init_searching(int player);

/**
 * tell to start this search.
 * @param pos The pos of the chess last put.
 */
void start_searching(pos_t pos);

/**
 * Collect the pos calculated to put.
 * @return the suggested pos.
 */
pos_t collect_value(void);

/**
 * stop search process.
 */
void stop_searching(void);

#endif //GOMOKU_SEARCH_H
