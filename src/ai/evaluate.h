//
// Created by liu on 19-1-1.
//

#ifndef GOMOKU_EVALUATE_H
#define GOMOKU_EVALUATE_H

#include <pthread.h>
#include <interface/board.h>

typedef struct search_node {
  int dist;
  int depth;
  int child_succeed;
  pthread_mutex_t lock;
  int searched;
  pos_t pos;
  int player;
  int (*get_chess_func)(pos_t pos, void *param);
  long long value;
  struct search_node *parent;
  struct search_node *prev;
  struct search_node *next;
  struct search_node *child;
} search_node_t;

/**
 * evaluate the board.
 * @param node the node to evaluate.
 * @return the value of the board.
 */
void evaluate(search_node_t *node);

/**
 * get a color of chess from the search node.
 * @param pos the pos to get.
 * @param node the node searching.
 * @return the color of chess, 1 black, 2 white.
 */
int get_chess_from_search_node(pos_t pos, void *node);

#endif //GOMOKU_EVALUATE_H
