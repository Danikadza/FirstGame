cmake_minimum_required(VERSION 3.28)

project(FirstGame LANGUAGES C VERSION 0.0.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE FirstGame_SRC src/*.c)
add_executable(${PROJECT_NAME} ${FirstGame_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC GS_ENGINE)