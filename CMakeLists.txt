cmake_minimum_required(VERSION 3.12)
project(gomoku C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    #set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib ${CMAKE_PREFIX_PATH})
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_PREFIX_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/log4c-1.2.4-${CMAKE_HOST_SYSTEM_PROCESSOR}-linux
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/expat-2.2.6-${CMAKE_HOST_SYSTEM_PROCESSOR}-linux)
endif ()

find_package(log4c REQUIRED)

find_package(EXPAT REQUIRED)

add_executable(gomoku
        src/main.c
        src/interface/board.c
        src/interface/screen.c
        src/utils/logger.c "src/utils/values.h" src/utils/values.c)

target_link_libraries(gomoku ${log4c_LIBRARIES})

target_include_directories(gomoku PUBLIC ${log4c_INCLUDE_DIRS})

target_link_libraries(gomoku ${EXPAT_LIBRARIES})

target_include_directories(gomoku PUBLIC ${EXPAT_INCLUDE_DIRS})

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_C_FLAGS -DWITHOUT_LOG4C)
endif ()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(gomoku pthread)
endif ()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_custom_command(TARGET gomoku POST_BUILD
            COMMAND copy ${CMAKE_CURRENT_SOURCE_DIR}/log4crc ${CMAKE_CURRENT_BINARY_DIR}
            COMMAND if not exist ${CMAKE_CURRENT_BINARY_DIR}/log md ${CMAKE_CURRENT_BINARY_DIR}/log
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/log4crc
            BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/log4crc
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Copying the log4crc file.")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_custom_command(TARGET gomoku POST_BUILD
            COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/log4crc ${CMAKE_CURRENT_BINARY_DIR}
            COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/log
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/log4crc
            BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/log4crc
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Copying the log4crc file.")
endif ()
