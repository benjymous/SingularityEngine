cmake_minimum_required(VERSION 2.8.9)
project (tetronimo)

find_package(glfw3 3.3 REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

include_directories(../include ../../../include ../../../externals/glad/include)

add_executable(tetronimo ../../../core/GlfwMain.cpp ../../../examples/tetronimo/tetronimo.c ../../../externals/glad/src/glad.c)

target_link_libraries(tetronimo glfw)
target_link_libraries(tetronimo OpenGL::GL)
target_link_libraries(tetronimo ${CMAKE_DL_LIBS})

