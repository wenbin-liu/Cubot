cmake_minimum_required(VERSION 3.2)

project(Cubot)

find_package(OpenCV REQUIRED)

#use c++ 11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#debug mode, use gdb to debug
SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

include_directories(
 inc
 /usr/local/include
 kociemba_genLib/include
 ${OpenCV_INCLUDE_DIRS}
)


add_subdirectory(kociemba_genLib)

set(testSrcList
    src/main.cpp
    src/cube.cpp
    src/uart.c
    src/attemp_noturn.cpp
    )




add_executable(main ${testSrcList})


target_link_libraries(main kociemba pthread ${OpenCV_LIBS})

