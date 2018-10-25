//
// Created by liu on 18-10-13.
//

#include "utils/thread.h"
#include <locale.h>
#include "utils/logger.h"
#include "interface/screen.h"
#include "interface/board.h"

thrd_t game;

int main() {
    setlocale(LC_ALL, "");
    int game_return_code;
    initLogger();
    initScreen();
    initBoard();
    info("game begin!");
    if (thrd_create(&game, loop, NULL) != thrd_success) {
        error("Thread create failed.")
    }
    thrd_join(game, &game_return_code);
    info("Game end.")
    log4c_fini();
    return game_return_code;
}
