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
 ${OpenCV_INCLUDE_DIRS}
)



set(testSrcList
    src/main.cpp
    src/cube.cpp
    )




add_executable(main ${testSrcList})


target_link_libraries(main ${OpenCV_LIBS} )

