cmake_minimum_required(VERSION 3.25)
project(C-Pong-SDL C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2-2.28.2\\x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(C-Pong-SDL main.c Ball.c Ball.h Collision.c Collision.h SharpKeywords.h DeltaTime.c DeltaTime.h ScreenTransitions.c ScreenTransitions.h)


target_link_libraries(C-Pong-SDL ${SDL2_LIBRARY})
