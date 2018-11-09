//
// Created by liu on 18-10-18.
//

#ifndef GOMOKU_LOGGER_H
#define GOMOKU_LOGGER_H

#include <log4c.h>

void initLogger();

extern log4c_category_t *category;

#define error(msg, args...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_ERROR, msg, ##args);\
}

#define warn(msg, args...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_WARN, msg, ##args);\
}

#define info(msg, args...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_INFO, msg, ##args);\
}

#define debug(msg, args...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_DEBUG, msg, ##args);\
}

#define trace(msg, args...) {\
  const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
  log4c_category_log_locinfo(category, &locinfo, LOG4C_PRIORITY_TRACE, msg, ##args);\
}

#endif //GOMOKU_LOGGER_H
