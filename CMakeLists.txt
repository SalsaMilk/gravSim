cmake_minimum_required(VERSION 3.20)
project(gravsim)

set(CMAKE_CXX_STANDARD 17)

add_executable(gravsim main.cpp object.h)

file(GLOB LIBS
        lib/*.lib)

target_link_libraries(gravsim
        ${LIBS}
)