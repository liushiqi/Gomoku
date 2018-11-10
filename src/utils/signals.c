//
// Created by liu on 18-11-4.
//

#include <stdatomic.h>
#include <signal.h>
#include <utils/logger.h>
#include "utils/thread.h"
#include "signals.h"

int intSignal;
pthread_rwlock_t intSignalMutex;

int getIntSignal() {
  pthread_rwlock_rdlock(&intSignalMutex);
  int sig = intSignal;
  pthread_rwlock_unlock(&intSignalMutex);
  return sig;
}

void setIntSignal(int signal) {
  pthread_rwlock_wrlock(&intSignalMutex);
  intSignal = signal;
  pthread_rwlock_unlock(&intSignalMutex);
}

void initSignal() {
  struct sigaction intAction = {.sa_handler = sigintHandler};
  sigaction(SIGINT, &intAction, 0);
  pthread_rwlock_init(&intSignalMutex, NULL);
}

void sigintHandler(num) {
  debug("sigint received.")
  pthread_rwlock_wrlock(&intSignalMutex);
  intSignal = 1;
  pthread_rwlock_unlock(&intSignalMutex);
}

void signalOnExit() {
  pthread_rwlock_destroy(&intSignalMutex);
}
