#include <stdio.h>
#include <lzma.h>
#include <ctype.h>
#include <stdlib.h>
#include "interface/interface.h"

int main() {
    wchar_t str[10];
    initBoard();
    int end = 0;
    while (!(winner() || end)) {
        int line = -1, row = -1;
        displayBoard();
    }
    return 0;
}