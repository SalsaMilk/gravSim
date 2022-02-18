cmake_minimum_required(VERSION 3.20)
project(gravsim)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE
        *.cpp
        SDL/*.c
        )

add_executable(gravsim ${SOURCE})

file(GLOB LIBS
        lib/*.lib)

target_link_libraries(gravsim
        ${LIBS}
)