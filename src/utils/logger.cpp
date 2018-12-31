//
// Created by liu on 18-10-18.
//

#include <cwchar>
#include <cwctype>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <cstdarg>
#include <log4cplus/helpers/snprintf.h>
extern "C" {
#include "logger.h"
}

log4cplus::Logger *logger;

GOMOKU_EXTERN_C void init_logger(void) {
  try {
    log4cplus::initialize();
    log4cplus::PropertyConfigurator configurator(LOG4CPLUS_C_STR_TO_TSTRING("./log4cplus.properties"));
    configurator.configure();
    logger = new log4cplus::Logger(log4cplus::Logger::getRoot());
  } catch (std::exception const &) {
    std::wprintf(L"log4cplus.properties not found or wrong format.");
    exit(1);
  }
}

GOMOKU_EXTERN_C void log_fmt(const char *file, int line, const char *func, log4cplus_loglevel_t ll,
                             const log4cplus_char_t *fmt, ...) {
  int ret_val;
  try {
    if (logger->isEnabledFor(ll)) {
      const log4cplus::tchar *msg = nullptr;
      log4cplus::helpers::snprintf_buf buf;
      std::va_list ap;
      do {
        va_start(ap, fmt);
        ret_val = buf.print_va_list(msg, fmt, ap);
        va_end(ap);
      } while (ret_val == -1);
      logger->forcedLog(ll, msg, file, line, func);
    }
  } catch (std::exception const &) {
    // IGNORE
  }
}

GOMOKU_EXTERN_C void fin_logger(void) {
  delete logger;
  log4cplus_shutdown();
}