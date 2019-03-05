cmake_minimum_required(VERSION 3.2) 
project(kociemba_genLib) 

# specify source and include directory
aux_source_directory(. DIR_SRCS) 
include_directories(include) 

# generate dynamic library
#add_library(kociemba SHARED ${DIR_SRCS}) 
#set_target_properties(kociemba PROPERTIES VERSION 1.0 SOVERSION 1)

# generate static library
add_library(kociemba STATIC ${DIR_SRCS}) 
set_target_properties(kociemba PROPERTIES OUTPUT_NAME "kociemba")
