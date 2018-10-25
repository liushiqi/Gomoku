//
// Created by liu on 18-10-17.
//

#include <locale.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include "screen.h"
#include "board.h"
#include "../utils/logger.h"
#include "../utils/values.h"

/** 0 if is single player, 1 if is multi player. */
int gameType;

/** 1 is black, 2 is white. */
int player;

/**
 * Request a int for the given request from standard input.
 * @param request The request string.
 * @param value The value to receive.
 * @return 1 if read successfully, otherwise 0.
 */
int requestInt(const wchar_t *request, int *value);

/**
 * Request a wide string for the given request from standard input.
 * @param request The request string.
 * @param value The value to receive.
 * @param count The max length of a line.
 * @return 1 if read successfully, otherwise 0.
 */
int requestLine(const wchar_t *request, wchar_t *value, int count);

/**
 * Get the input from either standard input or the AI calc result and put a chess.
 * @return 0 if succeed, 1 if black win, 2 if white win, -1 if interrupted.
 */
int duUserInput();

void initScreen() {
    int errorCode;
    clear();
    wprintf(numberOfPlayersRequest);
    while ((errorCode = requestInt(numberRequest, &gameType)) != 0 && (gameType != 1 && gameType != 2)) {
        warn("Wrong input %d", gameType);
        wprintf(wrongInput);
    }
    if (errorCode == -1) {
        warn("Exited unexpectedly.")
        exit(0);
    }
    --gameType;
    info("Game mode is %s player.", gameType ? "single" : "multi")
    if (!gameType) {
        wprintf(typeOfChessRequest);
        while ((errorCode = requestInt(numberRequest, &player)) != 0 && (player != 1 && player != 2)) {
            warn("Wrong input %d", player);
            wprintf(wrongInput);
        }
        if (errorCode == -1) {
            warn("Exited unexpectedly.")
            exit(0);
        }
        info("player chose %s.", player - 1 ? "black" : "white")
    }
    info("Screen initialized.")
}

void refresh() {
    clear();
    wprintf(header);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        wprintf(L"%02d  ", i + 1);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Pos pos = {i, j};
            switch (getChess(pos)) {
                case 1:
                    if (!posCmp(pos, getLastPos())) putwchar(lastBlack);
                    else putwchar(black);
                    break;
                case 2:
                    if (!posCmp(pos, getLastPos())) putwchar(lastWhite);
                    else putwchar(white);
                    break;
                case 0:
                    if (pos.x == 0)
                        putwchar(pos.y == 0 ? leftTop : pos.y == BOARD_SIZE - 1 ? rightTop : top);
                    else if (pos.x == BOARD_SIZE - 1)
                        putwchar(pos.y == 0 ? leftGround : pos.y == BOARD_SIZE - 1 ? rightGround : ground);
                    else
                        putwchar(pos.y == 0 ? left : pos.y == BOARD_SIZE - 1 ? right : middle);
                    break;
                default: error("This case should not happen!!!! the value of chess is %d", getChess(pos))
                    break;
            }
        }
        putwchar('\n');
    }
}

int loop(void *ptr) {
    while (1) {
        int result;
        refresh();
        result = duUserInput();
        switch (result) {
            case 0:
                continue;
            case 1: info("Black win!!!")
                wprintf(blackWin);
                return 0;
            case 2: info("White win!!!")
                wprintf(whiteWin);
                return 0;
            case -1: info("Program quited with no one wins.")
                wprintf(quit);
                return 0;
        }
    }
}

void clear() {
    wprintf(L"\033c");
}

int duUserInput() {
    wchar_t line[1000];
    Pos pos = {-1, -1};
    if (gameType || getPlayer() == player) {
        wprintf(direct);
        while (requestLine(player - 1 ? blackTerm : whiteTerm, line, 1000) != 0) {
            trace("Input string is %ls", line);
            if (!wcscmp(line, L"quit\n")) {
                return -1;
            }
            if (iswalpha((wint_t) line[0])) {
                pos.y = towlower((wint_t) line[0]) - 'a';
                pos.x = (int) wcstol(line + 1, NULL, 10) - 1;
            } else {
                warn("Wrong input %s", line);
                wprintf(wrongInput);
                continue;
            }+
            switch (putChess(pos)) {
                case 0:
                    return 0;
                case 1:
                    return 1;
                case 2:
                    return 2;
                case -1:
                    wprintf(forbidden);
                    continue;
                case -2:
                    wprintf(wrongInput);
                    continue;
                default: error("This should not happen!")
                    break;
            }
        }
        return -1;
    } else {
        // TODO AI input
        skip();
        return 0;
    }
}

int requestInt(const wchar_t *request, int *value) {
    wchar_t *result;
    wchar_t line[1000];
    wprintf(request);
    while ((result = fgetws(line, 1000, stdin)) != NULL && !iswdigit((wint_t) line[0])) {
        warn("Wrong input %s", line);
        wprintf(wrongInput);
        wprintf(request);
    }
    if (result == NULL) {
        info("EOF found.")
        return 0;
    }
    *value = (int) wcstol(line, NULL, 10);
    return 1;
}

int requestLine(const wchar_t *request, wchar_t *value, int count) {
    wprintf(request);
    if (fgetws(value, count, stdin) != NULL) return 1;
    else return 0;
}
