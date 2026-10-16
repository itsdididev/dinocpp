cmake_minimum_required(VERSION 3.14)
project(dino_offline CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.0)
FetchContent_MakeAvailable(raylib)

add_executable(dino main.cpp)
target_link_libraries(dino raylib)
