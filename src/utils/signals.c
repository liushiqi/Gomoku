//
// Created by liu on 18-11-4.
//

#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include "signals.h"
#include "logger.h"

static int sigint_status;
static pthread_rwlock_t sigint_mutex;

/**
 * The handler of sigint.
 */
void sigint_handler(int signal);

int get_sigint_status() {
  pthread_rwlock_rdlock(&sigint_mutex);
  int sig = sigint_status;
  pthread_rwlock_unlock(&sigint_mutex);
  return sig;
}

void set_sigint_status(int signal) {
  pthread_rwlock_wrlock(&sigint_mutex);
  sigint_status = signal;
  pthread_rwlock_unlock(&sigint_mutex);
}

void init_signal() {
  pthread_rwlock_init(&sigint_mutex, NULL);
  struct sigaction action = {.sa_handler = sigint_handler};
  set_sigint_status(0);
  sigaction(SIGINT, &action, NULL);
}

void sigint_handler(__attribute__((unused)) int signal) {
  set_sigint_status(1);
}

void fin_signal() {}
