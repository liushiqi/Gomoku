cmake_minimum_required(VERSION 3.12)
project(gomoku)

cmake_policy(SET CMP0077 NEW)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        src/interface/board.c
        src/interface/screen.c
        src/interface/detector.c
        src/utils/logger.cpp
        src/utils/values.c
        src/utils/signals.c)

add_executable(gomoku ${SOURCES} src/main.c)
target_include_directories(gomoku PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(gomoku PRIVATE -Wall -Wextra)

set(UNICODE ON CACHE BOOL "UNICODE")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "SHARED")
add_subdirectory(lib/log4cplus)
target_link_libraries(gomoku log4cplusSU)
target_include_directories(gomoku PRIVATE lib/log4cplus/include ${CMAKE_BINARY_DIR}/lib/log4cplus/include)
target_compile_definitions(gomoku PRIVATE UNICODE)

set_property(TARGET gomoku PROPERTY C_STANDARD 11)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(log_priority DEBUG)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(log_priority ERROR)
endif ()

set(TEST_FILES ${SOURCES} test/test.cpp)
find_package(Boost 1.55.5 COMPONENTS unit_test_framework)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(testgomoku ${TEST_FILES})
    target_link_libraries(testgomoku PUBLIC ${Boost_LIBRARIES})
    target_include_directories(testgomoku PUBLIC src)
    target_compile_options(testgomoku PUBLIC -Wall -Wextra)
    target_link_libraries(testgomoku PUBLIC log4cplusSU)
    target_include_directories(testgomoku PRIVATE lib/log4cplus/include ${CMAKE_BINARY_DIR}/lib/log4cplus/include)
    target_compile_definitions(testgomoku PRIVATE UNICODE)
    enable_testing()
    add_test(test testgomoku)
endif ()

configure_file(log4cplus.properties.in log4cplus.properties)
