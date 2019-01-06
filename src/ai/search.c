//
// Created by liu on 19-1-4.
//

#include <stdlib.h>
#include <interface/detector.h>
#include <utils/logger.h>
#include "search.h"
#include "evaluate.h"

static int computer_player;

/**
 * The part of stop type.
 */
typedef enum { NORMAL, GET_VALUE, SET_VALUE, QUIT } stop_type_t;
static stop_type_t stop_type;
static pthread_rwlock_t type_lock;

void set_stop_type(stop_type_t type) {
  pthread_rwlock_wrlock(&type_lock);
  stop_type = type;
  pthread_rwlock_unlock(&type_lock);
}

stop_type_t get_stop_type(void) {
  pthread_rwlock_rdlock(&type_lock);
  stop_type_t sig = stop_type;
  pthread_rwlock_unlock(&type_lock);
  return sig;
}

/**
 * search thread.
 */
typedef struct {
  search_node_t root;
  pthread_t search_thread;
  search_node_t *searching_node;
  search_node_t *search_root;
  pos_t search_pos;
  stop_type_t stop_type;
  pthread_rwlock_t type_lock;
} search_thread_t;
search_thread_t threads[BOARD_SIZE * BOARD_SIZE + 1];
search_thread_t root;

void set_root_position(search_thread_t *pos, stop_type_t type, search_node_t *node) {
  pthread_rwlock_wrlock(&pos->type_lock);
  if (type == SET_VALUE)
    threads->search_root = node;
  pos->stop_type = type;
  pthread_rwlock_unlock(&pos->type_lock);
}

stop_type_t get_thread_stop_type(search_thread_t *pos) {
  pthread_rwlock_wrlock(&pos->type_lock);
  stop_type_t stop = pos->stop_type;
  pthread_rwlock_unlock(&pos->type_lock);
  return stop;
}

/**
 * The part of last position
 */
static pos_t last_position;
static pthread_rwlock_t last_position_lock;

void set_last_position(pos_t pos) {
  pthread_rwlock_wrlock(&last_position_lock);
  last_position = pos;
  pthread_rwlock_unlock(&last_position_lock);
}

pos_t get_last_position(void) {
  pthread_rwlock_rdlock(&last_position_lock);
  pos_t pos = last_position;
  pthread_rwlock_unlock(&last_position_lock);
  return pos;
}

/**
 * the part of result pos.
*/
static pos_t result_pos;
static pthread_cond_t result_cond;
static pthread_mutex_t result_mutex;
static pthread_rwlock_t result_pos_lock;

void set_result_pos(pos_t pos) {
  pthread_rwlock_wrlock(&result_pos_lock);
  result_pos = pos;
  pthread_rwlock_unlock(&result_pos_lock);
}

pos_t get_result_pos(void) {
  pthread_rwlock_rdlock(&type_lock);
  pos_t pos = result_pos;
  pthread_rwlock_unlock(&type_lock);
  return pos;
}

typedef struct {
  pos_t pos;
  long long value;
} return_value_t;

return_value_t get_best_pos(search_node_t *node) {
  if (node->child == NULL) {
    return_value_t max = {node->pos, node->value};
    if (node->searched == 0) {
      return_value_t min = {node->pos, -9223372036854775807};
      return min;
    }
    return max;
  }
  search_node_t *child = node->child;
  return_value_t max = get_best_pos(child);
  while (child->next != NULL) {
    child = child->next;
    if (get_best_pos(child).value > max.value) {
      max.value = child->value;
      max.pos = child->pos;
    }
  }
  max.value = -max.value;
  return max;
}

int pos_search_valid(pos_t pos, int (*get_chess_func)(pos_t, void *), void *param, int dist) {
  if (get_chess_func(pos, param) != 0) return 0;
  for (int i = 0; i < 8; ++i) {
    pos_t searching = pos;
    for (int j = 0; j < 2; ++j) {
      searching = pos_add(searching, dirs[i]);
      if (get_chess_func(searching, param) > 0) {
        if (j == 1 && dist == 2) continue;
        else return j + 1;
      }
    }
  }
  return 0;
}

int new_child(search_thread_t *thread, search_node_t *node) {
  search_node_t *new_child = (search_node_t *) malloc(sizeof(search_node_t));
  new_child->parent = node;
  new_child->depth = new_child->parent->depth + 1;
  new_child->player = (node->player == 1) ? 2 : 1;
  new_child->child = NULL;
  new_child->next = NULL;
  new_child->prev = NULL;
  new_child->searched = 0;
  new_child->get_chess_func = get_chess_from_search_node;
  pthread_mutex_init(&new_child->lock, NULL);
  for (int i = 0; i < 15; ++i) {
    int get = 0;
    for (int j = 0; j < 15; ++j) {
      pos_t pos = {i, j};
      int k;
      if ((k = pos_search_valid(pos, get_chess_from_search_node, node, node->dist)) > 0) {
        new_child->dist = k;
        get = 1;
        new_child->pos = pos;
        break;
      }
    }
    if (get == 1) break;
    else if (i == 15) {
      thread->search_pos.x = 0;
      thread->search_pos.y = 0;
      free(new_child);
      return 0;
    }
  }
  node->child = new_child;
  thread->searching_node = new_child;
  thread->search_pos = new_child->pos;
  TRACE(L"Searching (%d, %d)", thread->search_pos.x, thread->search_pos.y);
  return 1;
}

int new_next_node(search_thread_t *thread, int k) {
  search_node_t *new_node = (search_node_t *) malloc(sizeof(search_node_t));
  new_node->parent = thread->searching_node->parent;
  new_node->depth = new_node->parent->depth + 1;
  new_node->player = thread->searching_node->player;
  new_node->child = NULL;
  new_node->next = NULL;
  new_node->dist = (k == 1 && thread->searching_node->parent->dist == 1) ? 1 : 2;
  new_node->prev = thread->searching_node;
  new_node->get_chess_func = get_chess_from_search_node;
  new_node->pos = thread->search_pos;
  new_node->searched = 0;
  pthread_mutex_init(&new_node->lock, NULL);
  thread->searching_node->next = new_node;
  thread->searching_node = new_node;
  thread->search_pos = new_node->pos;
  TRACE(L"Searching (%d, %d)", thread->search_pos.x, thread->search_pos.y);
  return 1;
}

int next_searching_node(search_thread_t *thread) {
  if (thread->searching_node == thread->search_root) {
    search_node_t *child = thread->searching_node;
    while (child->child != NULL) child = child->child;
    return new_child(thread, child);
  } else if (thread->search_pos.x == BOARD_SIZE - 1) {
    if (thread->search_pos.y == BOARD_SIZE - 1) {
      thread->searching_node = thread->searching_node->parent;
      thread->search_pos = thread->searching_node->pos;
      return next_searching_node(thread);
    } else {
      thread->search_pos.x = 0;
      ++thread->search_pos.y;
    }
  } else {
    ++thread->search_pos.x;
  }
  if (thread->searching_node->next != NULL) {
    thread->searching_node = thread->searching_node->next;
    return new_child(thread, thread->searching_node);
  } else if (thread->searching_node->prev == NULL || thread->searching_node->prev->child == NULL) {
    int k =
        pos_search_valid(thread->search_pos,
                         get_chess_from_search_node,
                         thread->searching_node->parent,
                         thread->searching_node->parent->dist);
    if (k > 0)
      return new_next_node(thread, k);
    else return 0;
  } else {
    return new_child(thread, thread->searching_node);
  }
}

void *search_loop(__attribute__((unused)) void *param) {
  search_thread_t *thread = (search_thread_t *) param;
  pthread_rwlock_init(&thread->type_lock, NULL);
  while (get_thread_stop_type(param) != SET_VALUE) {
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 10000;
    nanosleep(&delay, &delay);
  }
  thread->root.get_chess_func = get_chess_helper;
  thread->searching_node = &thread->root;
  thread->search_root = &thread->root;
  thread->root.value = 0;
  thread->root.searched = 1;
  set_stop_type(NORMAL);
  while (1) {
    switch (get_stop_type()) {
      case NORMAL:
        if (next_searching_node(thread)) {
          evaluate(thread->searching_node);
        }
        break;
      case SET_VALUE: {
        pos_t pos = get_last_position();
        if (is_invalid_pos(pos)) {
          set_stop_type(NORMAL);
          break;
        }
        search_node_t *node = thread->search_root->child;
        while (pos_cmp(node->pos, pos)) {
          if (node->next == NULL) {
            search_node_t *new_node = (search_node_t *) malloc(sizeof(search_node_t));
            new_node->parent = node->parent;
            new_node->depth = new_node->parent->depth + 1;
            new_node->player = node->player;
            new_node->child = NULL;
            new_node->next = NULL;
            new_node->dist = 1;
            new_node->prev = node;
            new_node->get_chess_func = get_chess_helper;
            new_node->pos = thread->search_pos;
            new_node->searched = 0;
            pthread_mutex_init(&new_node->lock, NULL);
            node->next = new_node;
            node = new_node;
            break;
          } else node = node->next;
        }
        thread->search_root = node;
        thread->searching_node = node;
        thread->search_pos = node->pos;
        set_stop_type(NORMAL);
        break;
      }
      case GET_VALUE: {
        return_value_t value = get_best_pos(thread->search_root);
        ERROR(L"max value: %d", value.value);
        pos_t result = value.pos;
        if (get_chess(thread->root.pos) == 0) {
          result = thread->root.pos;
          DEBUG(L"result (%d, %d)", result.x, result.y);
          pthread_mutex_lock(&result_mutex);
          set_result_pos(result);
          pthread_cond_signal(&result_cond);
          pthread_mutex_unlock(&result_mutex);
          set_stop_type(NORMAL);
          continue;
        }
        pthread_mutex_lock(&result_mutex);
        set_result_pos(result);
        pthread_cond_signal(&result_cond);
        pthread_mutex_unlock(&result_mutex);
        set_last_position(result);
        set_stop_type(SET_VALUE);
        break;
      }
      case QUIT:return NULL;
    }
  }
}

void *loop_game(__attribute__((unused)) void *param) {
  if (computer_player == 1) {
    root.root.pos.x = 7;
    root.root.pos.y = 7;
  } else {
    while (get_stop_type() != SET_VALUE) {
      struct timespec delay;
      delay.tv_sec = 0;
      delay.tv_nsec = 10000;
      nanosleep(&delay, &delay);
    }
    root.root.pos = pos_add(get_last_position(), dirs[random() % 8]);
    set_stop_type(NORMAL);
  }
  root.root.get_chess_func = get_chess_helper;
  root.searching_node = &root.root;
  root.search_root = &root.root;
  root.root.value = 0;
  root.root.searched = 1;
  set_stop_type(NORMAL);
  for (int i = 0; i < BOARD_SIZE * BOARD_SIZE + 1; ++i) {
    pthread_create(&(threads + i)->search_thread, NULL, search_loop, threads + i);
  }
  new_child(&root, root.search_root);
  for (int i = 0; i < BOARD_SIZE * BOARD_SIZE + 1; ++i) {
    next_searching_node(&root);
    if (root.search_pos.x != 14 && root.search_pos.y != 14) {
      set_root_position(threads + i, SET_VALUE, root.searching_node);
    } else {
      break;
    }
  }
  while (1) {
    switch (get_stop_type()) {
      case NORMAL: {
        struct timespec delay;
        delay.tv_sec = 0;
        delay.tv_nsec = 100000000;
        nanosleep(&delay, &delay);
      }
      case SET_VALUE: {
        pos_t pos = get_last_position();
        if (is_invalid_pos(pos)) {
          set_stop_type(NORMAL);
          break;
        }
        search_node_t *node = root.search_root->child;
        if (node == NULL) {
          new_child(&root, root.searching_node);
          node = root.search_root->child;
        }
        while (pos_cmp(node->pos, pos)) {
          if (node->next == NULL) {
            search_node_t *new_node = (search_node_t *) malloc(sizeof(search_node_t));
            new_node->parent = node->parent;
            new_node->depth = new_node->parent->depth + 1;
            new_node->player = node->player;
            new_node->child = NULL;
            new_node->next = NULL;
            new_node->dist = 1;
            new_node->prev = node;
            new_node->get_chess_func = get_chess_helper;
            new_node->pos = root.search_pos;
            new_node->searched = 0;
            pthread_mutex_init(&new_node->lock, NULL);
            node->next = new_node;
            node = new_node;
            break;
          } else node = node->next;
        }
        root.search_root = node;
        root.searching_node = node;
        root.search_pos = node->pos;
        search_node_t *child_node = node->child;
        for (int i = 0; child_node != NULL && i < BOARD_SIZE * BOARD_SIZE + 1; ++i) {
          set_root_position(threads + i, SET_VALUE, child_node);
          child_node = child_node->next;
        }
        set_stop_type(NORMAL);
        break;
      }
      case GET_VALUE: {
        return_value_t value = get_best_pos(root.search_root);
        ERROR(L"max value: %d", value.value);
        pos_t result = value.pos;
        if (get_chess(root.root.pos) == 0) {
          result = root.root.pos;
          DEBUG(L"result (%d, %d)", result.x, result.y);
          pthread_mutex_lock(&result_mutex);
          set_result_pos(result);
          pthread_cond_signal(&result_cond);
          pthread_mutex_unlock(&result_mutex);
          set_stop_type(NORMAL);
          continue;
        }
        pthread_mutex_lock(&result_mutex);
        set_result_pos(result);
        pthread_cond_signal(&result_cond);
        pthread_mutex_unlock(&result_mutex);
        set_last_position(result);
        set_stop_type(SET_VALUE);
        break;
      }
      case QUIT: {
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE + 1; ++i) {
          set_root_position(threads + i, QUIT, NULL);
        }
        return NULL;
      }
    }
  }
}

void init_searching(int player) {
  computer_player = player;
  pthread_rwlock_init(&type_lock, NULL);
  pthread_cond_init(&result_cond, NULL);
  pthread_mutex_init(&result_mutex, NULL);
  root.root.player = player;
  root.root.depth = 1;
  root.root.parent = root.root.parent;
  root.root.next = NULL;
  root.root.prev = NULL;
  root.root.child = NULL;
  pthread_create(&root.search_thread, NULL, loop_game, NULL);
}

void start_searching(pos_t pos) {
  set_last_position(pos);
  set_stop_type(SET_VALUE);
}

pos_t collect_value(void) {
  set_stop_type(GET_VALUE);
  pthread_cond_wait(&result_cond, &result_mutex);
  pos_t result = get_result_pos();
  return result;
}

void stop_searching(void) {
  set_stop_type(QUIT);
  pthread_join(root.search_thread, NULL);
}
