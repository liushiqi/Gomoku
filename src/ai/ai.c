//
// Created by liu on 19-1-1.
//

#include "ai.h"
#include <pthread.h>
#include <stdlib.h>
#include <utils/logger.h>
#include <interface/detector.h>

pthread_t ai_thread;

enum INT_TYPE {
  CALC, QUIT
} int_type;
pthread_rwlock_t type_lock;

static pos_t result;
static pthread_cond_t result_cond;
static pthread_mutex_t result_mutex;

static pthread_cond_t interrupt_sleep;
static pthread_mutex_t interrupt_mutex;

void set_int_type(enum INT_TYPE type) {
  pthread_rwlock_wrlock(&type_lock);
  int_type = type;
  pthread_rwlock_unlock(&type_lock);
}

enum INT_TYPE get_int_type() {
  pthread_rwlock_rdlock(&type_lock);
  enum INT_TYPE sig = int_type;
  pthread_rwlock_unlock(&type_lock);
  return sig;
}

void set_pos(int x, int y) {
  pos_t pos = {.x = x, .y = y};
  result = pos;
}

pos_t get_pos(void) {
  pos_t pos = result;
  return pos;
}

void *ai_loop(__attribute__((unused)) void *param) {
  while (1) {
    pthread_cond_wait(&interrupt_sleep, &interrupt_mutex);
    if (get_int_type() == QUIT) {
      break;
    } else {
      struct timespec delay;
      delay.tv_sec = 0;
      delay.tv_nsec = 1000000;
      nanosleep(&delay, &delay);
      pos_t pos = {(int) (random() % BOARD_SIZE), (int) (random() % BOARD_SIZE)};
      while (get_chess(pos) != 0 || is_forbidden(pos, get_player(), get_chess_helper, NULL, 0)) {
        pos.x = (int) (random() % BOARD_SIZE);
        pos.y = (int) (random() % BOARD_SIZE);
      }
      pthread_mutex_lock(&result_mutex);
      pthread_cond_signal(&result_cond);
      set_pos(pos.x, pos.y);
      pthread_mutex_unlock(&result_mutex);
    }
  }
  return NULL;
}

void init_ai(void) {
  pthread_cond_init(&interrupt_sleep, NULL);
  pthread_cond_init(&result_cond, NULL);
  pthread_rwlock_init(&type_lock, NULL);
  pthread_mutex_init(&interrupt_mutex, NULL);
  pthread_mutex_init(&result_mutex, NULL);
  set_pos(-1, -1);
  pthread_create(&ai_thread, NULL, ai_loop, NULL);
  DEBUG(L"AI init succeed.");
}

pos_t get_ai_input(void) {
  pthread_mutex_lock(&interrupt_mutex);
  set_int_type(CALC);
  pthread_cond_signal(&interrupt_sleep);
  pthread_mutex_unlock(&interrupt_mutex);
  DEBUG(L"Calc begin");
  pthread_cond_wait(&result_cond, &result_mutex);
  return get_pos();
}

void fin_ai(void) {
  pthread_mutex_lock(&interrupt_mutex);
  set_int_type(QUIT);
  pthread_cond_signal(&interrupt_sleep);
  pthread_mutex_unlock(&interrupt_mutex);
  pthread_join(ai_thread, NULL);
}
