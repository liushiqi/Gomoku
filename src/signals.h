//
// Created by liu on 18-11-4.
//

#ifndef GOMOKU_HELPER_H
#define GOMOKU_HELPER_H

/**
 * @return 1 if sigint received.
 */
int getIntSignal();

/**
 * set the intSignal value.
 */
void setIntSignal(int signal);

/**
 * init signal handler module.
 */
void initSignal();

/**
 * The handler of sigint.
 */
void sigintHandler(int num);

/**
 * clean signal environment on exit.
 */
void signalOnExit();

#endif //GOMOKU_HELPER_H
