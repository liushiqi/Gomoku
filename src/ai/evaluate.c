//
// Created by liu on 19-1-1.
//

#include <asm/errno.h>
#include <interface/detector.h>
#include <stdlib.h>
#include "evaluate.h"

/**
 * test whether a pos is a valid pos.
 * @param pos the pos to test.
 * @return 0 if invalid, 1 if valid.
 */
int is_valid_pos(pos_t pos) {
  return pos.x >= 0 && pos.x < BOARD_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE;
}

int get_chess_from_search_node(pos_t pos, void *node) {
  search_node_t *search_node = (search_node_t *) node;
  if (!pos_cmp(pos, search_node->pos)) return search_node->player;
  else return search_node->get_chess_func(pos, search_node->parent);
}

/**
 * evaluate the value of one point of the given position.
 * @param node the node to search.
 * @param pos the position to search.
 * @return the value of the point.
 */
long long evaluate_one_point(search_node_t *node, pos_t pos) {
  long long value = 0, three = 0, four = 0;
  for (long long i = 0; i < 8; ++i) {
    switch (search_chess_one_dir(pos, dirs[i], get_chess_from_search_node, node, 1)) {
      case 0:value += 0;
        break;
      case 1:value += 1;
        break;
      case 2:value += 100;
        break;
      case 3:value += 10000;
        three += 1;
        break;
      case 4:value += 1000000;
        four += 1;
        break;
      case 5:value += 100000000;
        break;
      default:if (node->player == 1) return -100000; else value += 10000;
    }
  }
  if (node->player == 1 && (three >= 4 || four >= 4)) return -100000; else return value;
}

void evaluate(search_node_t *node) {
  if (pthread_mutex_trylock(&node->lock) == EBUSY) { /* EMPTY */ }
  else if (node->searched == 1) {
    pthread_mutex_unlock(&node->lock);
  } else {
    if (node->parent != NULL) {
      node->value = -2 * node->parent->value;
      node->value += evaluate_one_point(node, node->pos);
      for (int i = 0; i < 8; ++i) {
        int j = 0;
        pos_t pos = pos_add(node->pos, dirs[i]);
        for (; j <= 5 && is_valid_pos(pos) && get_chess_from_search_node(pos, node) > 0;
               (pos = pos_add(pos, dirs[i])), ++j) {
          if (get_chess_from_search_node(pos, node) != node->player) {
            node->value += evaluate_one_point(node->parent, pos);
            node->value -= evaluate_one_point(node, pos);
          } else {
            node->value -= evaluate_one_point(node->parent, pos);
            node->value += evaluate_one_point(node, pos);
          }
        }
      }
    } else {
      node->value = 0;
      node->value += evaluate_one_point(node, node->pos);
      for (int i = 0; i < 8; ++i) {
        int j = 0;
        for (pos_t pos = node->pos; j <= 5 && is_valid_pos(pos) && get_chess_from_search_node(pos, node) > 0;
             (pos = pos_add(pos, dirs[i])), ++j) {
          if (get_chess_from_search_node(pos, node) != node->player) { node->value -= evaluate_one_point(node, pos); }
          else { node->value += evaluate_one_point(node, pos); }
        }
      }
    }
    node->searched = 1;
    pthread_mutex_unlock(&node->lock);
  }
}
