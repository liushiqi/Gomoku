//
// Created by liu on 18-10-13.
//

#include <locale.h>
#include <utils/logger.h>
#include <interface/board.h>
#include <interface/screen.h>
#include <signal.h>
#include "utils/thread.h"
#include "utils/signals.h"

thrd_t game;

int main() {
  setlocale(LC_ALL, "");
  int game_return_code;
  initSignal();
  initLogger();
  initScreen();
  initBoard();
  info("game begin!");
  if (thrd_create(&game, loop, NULL) != thrd_success) {
    error("Thread create failed.")
    return 1;
  }
  sigset_t mask;
  sigaddset(&mask, SIGINT);
  pthread_sigmask(SIG_BLOCK, &mask, NULL);
  thrd_join(game, &game_return_code);
  info("Game end.")
  log4c_fini();
  signalOnExit();
  return game_return_code;
}
