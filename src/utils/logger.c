//
// Created by liu on 18-10-18.
//

#include <log4c.h>
#include <string.h>
#include <time.h>
#include "logger.h"

log4c_category_t *category;

const char *detailedLayout(__attribute__((unused)) const log4c_layout_t *a_layout,
                           const log4c_logging_event_t *a_event) {
  char time[1000] = {0};
  static char buffer[4096];
  strftime(time, 1000, "%F %T", localtime(&a_event->evt_timestamp.tv_sec));
  sprintf(buffer, "[%s] %s.%03ld - %21s:%03d - %5s - %s\n", a_event->evt_category, time,
          a_event->evt_timestamp.tv_usec / 1000, strstr(a_event->evt_loc->loc_file, "src/") + 4,
          a_event->evt_loc->loc_line, log4c_priority_to_string(a_event->evt_priority),
          a_event->evt_msg);
  return buffer;
}

const log4c_layout_type_t detailed_layout = {"detailed", detailedLayout};

void initLogger() {
  log4c_layout_type_set(&detailed_layout);
  log4c_init();
  category = log4c_category_get("gomoku");
  info("logger initialized.")
}
