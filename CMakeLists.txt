cmake_minimum_required(VERSION 3.3)
project(run)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES headers.cpp main.cpp managers/user.cpp managers/admin.cpp managers/runner.cpp interface/commands.cpp interface/commandInterface.cpp)
add_executable(run ${SOURCE_FILES})