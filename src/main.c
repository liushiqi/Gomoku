#include <stdio.h>
#include <lzma.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include "interface/interface.h"

int main() {
    wchar_t line[100];
    initBoard();
    int end = 0;
    while (!(winner() || end)) {
        if (!(mode || (human ^ player))) {
            // TODO ROBOT
            player = !player;
        } else {
            displayBoard();
            requestInput();
            int x = 0, y = 0;
            while (wscanf(L"%ls", line)) {
                if (!wcscmp(line, L"-1q")) {
                    end = 1;
                    break;
                } else {
                    wchar_t *ptr;
                    x = (int) wcstol(line, &ptr, 10);
                    if (iswalpha((wint_t) *ptr)) {
                        y = towlower((wint_t) *ptr) - L'a' + 1;
                        if (setChess(x, y)) break;
                    }
                }
                wprintf(L"输入错误，请重新输入：");
            }
        }
    }
    return 0;
}