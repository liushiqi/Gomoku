# - Try to find log4c
# Once done this will define
#
#  log4c_FOUND - system has log4c
#  log4c_INCLUDE_DIRS - the log4c include directory
#  log4c_LIBRARIES - Link these to use log4c
#  log4c_DEFINITIONS - Compiler switches required for using log4c
#
#  Copyright (c) 2009 Andreas Schneider <mail@cynapses.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

find_path(log4c_INCLUDE_DIR NAMES log4c.h DOC "The log4c include directory")
find_library(log4c_LIBRARY NAMES log4c DOC "The log4c library")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(log4c DEFAULT_MSG log4c_LIBRARY log4c_INCLUDE_DIR)

if (log4c_FOUND)
    set(log4c_LIBRARIES ${log4c_LIBRARY})
    set(log4c_INCLUDE_DIRS ${log4c_INCLUDE_DIR})
endif ()

mark_as_advanced(log4c_INCLUDE_DIR log4c_LIBRARY)