cmake_minimum_required(VERSION 3.12)
project(gomoku C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(gomoku
        src/main.c
        src/interface/board.c
        src/interface/screen.c
        src/utils/logger.c
        src/utils/values.c)

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
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/log4c-1.2.4-x86_64-w64-mingw32 ${CMAKE_PREFIX_PATH})
    else ()
        set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/log4c-1.2.4-i686-w64-mingw32 ${CMAKE_PREFIX_PATH})
    endif ()
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_PREFIX_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/log4c-1.2.4-${CMAKE_HOST_SYSTEM_PROCESSOR}-linux-gnu)
endif ()
find_package(Log4C 1.2.4 REQUIRED)
target_link_libraries(gomoku ${Log4C_LIBRARIES})
target_include_directories(gomoku PUBLIC ${Log4C_INCLUDE_DIRS})

set_property(TARGET gomoku PROPERTY C_STANDARD 11)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        add_custom_command(OUTPUT log4crc
                DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/log4crc-debug
                COMMAND copy ${CMAKE_CURRENT_SOURCE_DIR}/log4crc-debug ${CMAKE_CURRENT_BINARY_DIR}/log4crc
                COMMAND if not exist ${CMAKE_CURRENT_BINARY_DIR}/log md ${CMAKE_CURRENT_BINARY_DIR}/log
                COMMENT "Copying the log4crc file.")
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        add_custom_command(OUTPUT log4crc
                DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/log4crc-debug
                COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/log4crc-debug ${CMAKE_CURRENT_BINARY_DIR}/log4crc
                COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/log
                COMMENT "Copying the log4crc file.")
    endif ()
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        add_custom_command(OUTPUT log4crc
                DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/log4crc-release
                COMMAND copy ${CMAKE_CURRENT_SOURCE_DIR}/log4crc-release ${CMAKE_CURRENT_BINARY_DIR}/log4crc
                COMMAND if not exist ${CMAKE_CURRENT_BINARY_DIR}/log md ${CMAKE_CURRENT_BINARY_DIR}/log
                COMMENT "Copying the log4crc file.")
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        add_custom_command(OUTPUT log4crc
                DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/log4crc-release
                COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/log4crc-release ${CMAKE_CURRENT_BINARY_DIR}/log4crc
                COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/log
                COMMENT "Copying the log4crc file.")
    endif ()
endif ()

add_custom_target(build ALL
        DEPENDS log4crc gomoku)