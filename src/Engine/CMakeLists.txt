cmake_minimum_required(VERSION 3.28)

project(GS_ENGINE LANGUAGES C VERSION 0.0.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_definitions(-DGS_ENGINE_COMPILE)

add_subdirectory(vendor/stb)

file(GLOB_RECURSE GS_ENGINE_SRC src/*.c)
add_library(${PROJECT_NAME} SHARED ${GS_ENGINE_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC STB)
target_include_directories(${PROJECT_NAME} PUBLIC include/)

if(LINUX)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${X11_LIBRARIES} m)
    target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIRS})
endif()
