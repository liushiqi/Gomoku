//
// Created by liu on 18-10-24.
//

#ifndef GOMOKU_THREAD_H
#define GOMOKU_THREAD_H

#if defined(__STDC_NO_THREADS__) || (__STDC_VERSION__ <= 199901L)

#include <tinycthread>

#elseif defined(MSVC)

#include <thr/threads.h>

#else

#include <threads.h>

#endif

#endif //GOMOKU_THREAD_H
