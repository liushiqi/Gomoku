//
// Created by liu on 18-10-18.
//

#ifndef GOMOKU_LOGGER_H
#define GOMOKU_LOGGER_H

#ifdef __cplusplus
#define GOMOKU_EXTERN_C extern "C"
#else // __cplusplus
#define GOMOKU_EXTERN_C
#endif

#include <wchar.h>
#include <wctype.h>
#include <log4cplus/clogger.h>

GOMOKU_EXTERN_C void init_logger(void);

GOMOKU_EXTERN_C void log_fmt(const char *file, int line, const char *func, log4cplus_loglevel_t ll,
                             const log4cplus_char_t *fmt, ...);

GOMOKU_EXTERN_C void fin_logger(void);

#define ERROR(...) do {\
  log_fmt(__FILE__, __LINE__, __FUNCTION__, L4CP_ERROR_LOG_LEVEL, __VA_ARGS__);\
} while(0)

#define WARN(...) do {\
  log_fmt(__FILE__, __LINE__, __FUNCTION__, L4CP_WARN_LOG_LEVEL, __VA_ARGS__);\
} while(0)

#ifndef RELEASE
#define INFO(...) do {\
  log_fmt(__FILE__, __LINE__, __FUNCTION__, L4CP_INFO_LOG_LEVEL, __VA_ARGS__);\
} while(0)

#define DEBUG(...) do {\
  log_fmt(__FILE__, __LINE__, __FUNCTION__, L4CP_DEBUG_LOG_LEVEL, __VA_ARGS__);\
} while(0)

#define TRACE(...) do {\
  log_fmt(__FILE__, __LINE__, __FUNCTION__, L4CP_TRACE_LOG_LEVEL, __VA_ARGS__);\
} while(0)
#else
#define INFO(...)

#define DEBUG(...)

#define TRACE(...)
#endif
#endif //GOMOKU_LOGGER_H
