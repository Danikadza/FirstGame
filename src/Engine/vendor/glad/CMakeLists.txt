cmake_minimum_required(VERSION 3.28)

project(GLAD C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

if(WIN32)
    set(GLAD_SRC src/gl.c src/wgl.c)
elseif(LINUX)
    set(GLAD_SRC src/gl.c src/glx.c)
endif()

add_library(${PROJECT_NAME} STATIC ${GLAD_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include/ ${OpenGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenGL_LIBRARIES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenGL_LIBRARIES} gdi32 opengl32)
endif()