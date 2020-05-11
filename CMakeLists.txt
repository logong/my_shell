cmake_minimum_required(VERSION 2.6)

project(my_shell)

message("Project Name: " ${PROJECT_NAME})

set(CMAKE_C_FLAGS "-g -Wall")

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE c_files ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${c_files})