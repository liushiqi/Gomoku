//
// Created by liu on 18-10-18.
//

#ifndef GOMOKU_LOGGER_H
#define GOMOKU_LOGGER_H

#include <log4c.h>

void initLogger(void);

extern log4c_category_t *category;

#define error(...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_ERROR, __VA_ARGS__);\
}

#define warn(...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_WARN, __VA_ARGS__);\
}

#define info(...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_INFO, __VA_ARGS__);\
}

#define debug(...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_DEBUG, __VA_ARGS__);\
}

#define trace(...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_TRACE, __VA_ARGS__);\
}

#endif //GOMOKU_LOGGER_H
