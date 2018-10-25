# - Try to find log4c
# Once done this will define
#
#  Log4C_FOUND - system has log4c
#  Log4C_INCLUDE_DIRS - the log4c include directory
#  Log4C_LIBRARIES - Link these to use log4c
#  Log4C_DEFINITIONS - Compiler switches required for using log4c
#
#  Copyright (c) 2009 Andreas Schneider <mail@cynapses.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

find_package(PkgConfig QUIET)

pkg_check_modules(PC_Log4C QUIET log4c)

find_path(Log4C_INCLUDE_DIR NAMES log4c.h DOC "The log4c include directory" HINTS ${PC_Log4C_INCLUDE_DIRS})
find_library(Log4C_LIBRARY NAMES log4c DOC "The log4c library" HINTS ${PC_Log4C_LIBRARY_DIRS})

if (Log4C_INCLUDE_DIR AND EXISTS "${Log4C_INCLUDE_DIR}/log4c/version.h")
    file(STRINGS "${Log4C_INCLUDE_DIR}/log4c/version.h" Log4C_version_str
            REGEX "^#[\t ]*define[\t ]+LOG4C_(MAJOR|MINOR|MICRO)_VERSION[\t ]+[0-9]+$")

    unset(Log4C_VERSION_STRING)
    foreach(VPART MAJOR MINOR MICRO)
        foreach(VLINE ${Log4C_version_str})
            if(VLINE MATCHES "^#[\t ]*define[\t ]+LOG4C_${VPART}_VERSION[\t ]+([0-9]+)$")
                set(Log4C_VERSION_PART "${CMAKE_MATCH_1}")
                if(Log4C_VERSION_STRING)
                    string(APPEND Log4C_VERSION_STRING ".${Log4C_VERSION_PART}")
                else()
                    set(Log4C_VERSION_STRING "${Log4C_VERSION_PART}")
               endif()
            endif()
        endforeach()
    endforeach()
endif ()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Log4C REQUIRED_VARS Log4C_LIBRARY Log4C_INCLUDE_DIR VERSION_VAR Log4C_VERSION_STRING)

if (Log4C_FOUND)
    set(Log4C_LIBRARIES ${Log4C_LIBRARY})
    set(Log4C_INCLUDE_DIRS ${Log4C_INCLUDE_DIR})

   if(NOT TARGET Log4C::Log4C)
        add_library(Log4C::Log4C UNKNOWN IMPORTED)
        set_target_properties(Log4C::Log4C PROPERTIES
                IMPORTED_LINK_INTERFACE_LANGUAGES "C"
                IMPORTED_LOCATION "${Log4C_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${Log4C_INCLUDE_DIRS}")
    endif()
endif ()

mark_as_advanced(Log4C_INCLUDE_DIR Log4C_LIBRARY)