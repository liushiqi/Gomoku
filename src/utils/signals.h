//
// Created by liu on 18-11-4.
//

#ifndef GOMOKU_HELPER_H
#define GOMOKU_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return 1 if sigint received.
 */
int get_sigint_status(void);

/**
 * set the intSignal value.
 */
void set_sigint_status(int signal);

/**
 * init signal handler module.
 */
void init_signal(void);

/**
 * clean signal environment on exit.
 */
void fin_signal(void);

#ifdef __cplusplus
}
#endif

#endif //GOMOKU_HELPER_H
