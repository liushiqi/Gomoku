//
// Created by liu on 19-1-1.
//

#include "ai.h"
#include "search.h"
#include <pthread.h>
#include <stdlib.h>
#include <utils/logger.h>

static pthread_t ai_thread;

typedef enum { CALC, QUIT } int_type_t;
static int_type_t int_type;
static pthread_rwlock_t type_lock;

static pos_t result;
static pthread_cond_t result_cond;
static pthread_mutex_t result_mutex;

static pthread_cond_t interrupt_sleep;
static pthread_mutex_t interrupt_mutex;

void set_int_type(int_type_t type) {
  pthread_rwlock_wrlock(&type_lock);
  int_type = type;
  pthread_rwlock_unlock(&type_lock);
}

int_type_t get_int_type() {
  pthread_rwlock_rdlock(&type_lock);
  int_type_t sig = int_type;
  pthread_rwlock_unlock(&type_lock);
  return sig;
}

void set_pos(pos_t pos) {
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
      stop_searching();
      break;
    } else {
      start_searching(get_last_pos());
      struct timespec delay;
      delay.tv_sec = 14;
      delay.tv_nsec = 0;
      nanosleep(&delay, &delay);
      pos_t pos = collect_value();
      pthread_mutex_lock(&result_mutex);
      pthread_cond_signal(&result_cond);
      set_pos(pos);
      pthread_mutex_unlock(&result_mutex);
    }
  }
  return NULL;
}

void init_ai(int player) {
  init_searching(player);
  pthread_cond_init(&interrupt_sleep, NULL);
  pthread_cond_init(&result_cond, NULL);
  pthread_rwlock_init(&type_lock, NULL);
  pthread_mutex_init(&interrupt_mutex, NULL);
  pthread_mutex_init(&result_mutex, NULL);
  pos_t pos = {-1, -1};
  set_pos(pos);
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
