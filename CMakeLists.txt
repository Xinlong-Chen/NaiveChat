cmake_minimum_required(VERSION 2.8)
project(NaiveChat)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -std=c++17")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/src/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/server)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/client)