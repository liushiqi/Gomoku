cmake_minimum_required(VERSION 3.12)
project(gomoku C)

set(CMAKE_C_STANDARD 11)

add_executable(gomoku
        src/main.c
        src/interface/board.c
        src/interface/screen.c
        src/utils/logger.c
        src/utils/values.c src/utils/signals.h src/utils/signals.c src/interface/boardHelper.c src/interface/boardHelper.h)
target_include_directories(gomoku PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# threads.h
include(CheckIncludeFile)
if (MSVC)
    check_include_file(thr/threads.h THREADS_EXIST)
else ()
    check_include_file(threads.h THREADS_EXIST)
endif ()
if (NOT ${THREADS_EXIST})
    add_subdirectory(lib/tinycthread)
    target_link_libraries(gomoku tinycthread)
    target_include_directories(gomoku lib/tinycthread/source)
endif ()
if (UNIX)
    target_link_libraries(gomoku pthread)
endif ()

# expat
option(BUILD_doc "" OFF)
option(BUILD_tests "" OFF)
add_subdirectory(lib/libexpat/expat)
target_link_libraries(gomoku expat)

# log4c
set(WITH_ROLLINGFILE ON)
add_subdirectory(lib/log4c)
target_link_libraries(gomoku log4c)
target_include_directories(gomoku PUBLIC lib/log4c/src)

set_property(TARGET gomoku PROPERTY C_STANDARD 11)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(log_priority debug)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(log_priority warn)
endif ()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log4crc.in
        ${CMAKE_CURRENT_SOURCE_DIR}/log4crc)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_custom_command(OUTPUT log4crc
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/log4crc
            COMMAND copy ${CMAKE_CURRENT_SOURCE_DIR}/log4crc ${CMAKE_CURRENT_BINARY_DIR}/log4crc
            COMMAND if not exist ${CMAKE_CURRENT_BINARY_DIR}/log md ${CMAKE_CURRENT_BINARY_DIR}/log
            COMMENT "Copying the log4crc file.")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_custom_command(OUTPUT log4crc
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/log4crc
            COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/log4crc ${CMAKE_CURRENT_BINARY_DIR}/log4crc
            COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/log
            COMMENT "Copying the log4crc file.")
endif ()

add_custom_target(build ALL
        DEPENDS log4crc gomoku)