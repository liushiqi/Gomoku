//
// Created by liu on 18-10-13.
//

#include <locale.h>
#include <utils/logger.h>
#include <interface/board.h>
#include <interface/screen.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <ai/ai.h>
#include "utils/signals.h"

static pthread_t game;

int main(void) {
  srandom((unsigned int) time(NULL));
  setlocale(LC_ALL, "");
  sigset_t mask;
  sigfillset(&mask);
  pthread_sigmask(SIG_BLOCK, &mask, NULL);
  init_logger();
  init_signal();
  init_screen();
  init_board();
  INFO(L"game begin!");
  if (pthread_create(&game, NULL, &loop, NULL) != 0) {
    ERROR(L"Thread create failed.");
    return 1;
  }
  pthread_join(game, NULL);
  INFO(L"Game end.");
  fin_signal();
  fin_ai();
  fin_logger();
  return EXIT_SUCCESS;
}
