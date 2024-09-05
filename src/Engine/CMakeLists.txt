cmake_minimum_required(VERSION 3.28)

project(GS_ENGINE LANGUAGES C VERSION 0.0.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(GS_OPENGL ON CACHE BOOL "OpenGL Backend")
set(GS_VULKAN OFF CACHE BOOL "Vulkan Backend")

add_compile_definitions(-DGS_ENGINE_COMPILE)

add_subdirectory(vendor/stb)

if(GS_OPENGL AND NOT GS_VULKAN)
    add_compile_definitions(-DGS_OPENGL_BACKEND)
    add_subdirectory(vendor/glad)
elseif(GS_VULKAN AND NOT GS_OPENGL)
    add_compile_definitions(-DGS_VULKAN_BACKEND)
else()
    message(FATAL_ERROR "GS_VULKAN AND GS_OPENGL Cant both be ON")
endif()

file(GLOB_RECURSE GS_ENGINE_SRC src/*.c)
add_library(${PROJECT_NAME} SHARED ${GS_ENGINE_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC STB)
target_include_directories(${PROJECT_NAME} PUBLIC include/)

if(LINUX)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${X11_LIBRARIES} m)
    target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIRS})
endif()

if(GS_OPENGL)
    target_link_libraries(${PROJECT_NAME} PUBLIC GLAD)
endif()
