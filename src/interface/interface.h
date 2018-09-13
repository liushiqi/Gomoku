//
// Created by liu on 18-9-7.
//

#ifndef CCLASS_INTERFACE_H
#define CCLASS_INTERFACE_H

#include <string.h>
#include <stdbool.h>

extern int board[21][21];
extern int mode;
extern int player;
int human;

void initBoard();

int winner();

void displayBoard();

void requestInput();

bool setChess(int x, int y);

#endif //CCLASS_INTERFACE_H
