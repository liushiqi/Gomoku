//
// Created by liu on 18-11-4.
//

#include <stdatomic.h>
#include <signal.h>
#include <utils/logger.h>
#include "utils/thread.h"
#include "signals.h"

int intSignal;
mtx_t intSignalMutex;

int getIntSignal() {
  mtx_lock(&intSignalMutex);
  int sig = intSignal;
  mtx_unlock(&intSignalMutex);
  return sig;
}

void setIntSignal(int signal) {
  mtx_lock(&intSignalMutex);
  intSignal = signal;
  mtx_unlock(&intSignalMutex);
}

void initSignal() {
  struct sigaction intAction = {.sa_handler = sigintHandler};
  sigaction(SIGINT, &intAction, 0);
  mtx_init(&intSignalMutex, mtx_recursive | mtx_timed);
}

void sigintHandler(num) {
  debug("sigint received.")
  mtx_lock(&intSignalMutex);
  intSignal = 1;
  mtx_unlock(&intSignalMutex);
}

void signalOnExit() {
  mtx_destroy(&intSignalMutex);
}
