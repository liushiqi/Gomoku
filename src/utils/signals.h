//
// Created by liu on 18-11-4.
//

#ifndef GOMOKU_HELPER_H
#define GOMOKU_HELPER_H

/**
 * @return 1 if sigint received.
 */
int getIntSignal(void);

/**
 * set the intSignal value.
 */
void setIntSignal(int signal);

/**
 * init signal handler module.
 */
void initSignal(void);

/**
 * The handler of sigint.
 */
void sigintHandler(int signal);

/**
 * clean signal environment on exit.
 */
void signalOnExit(void);

#endif //GOMOKU_HELPER_H
