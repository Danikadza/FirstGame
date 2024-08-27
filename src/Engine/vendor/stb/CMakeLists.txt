cmake_minimum_required(VERSION 3.28)

project(STB C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC stb.c)
target_include_directories(${PROJECT_NAME} PUBLIC include/)