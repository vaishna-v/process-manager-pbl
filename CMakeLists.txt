cmake_minimum_required(VERSION 3.10)
project(ProcessMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
include_directories(include ${CURSES_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp")

add_executable(ProcessMonitor ${SOURCES})
target_link_libraries(ProcessMonitor ${CURSES_LIBRARIES})