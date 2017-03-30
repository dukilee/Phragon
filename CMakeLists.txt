cmake_minimum_required(VERSION 2.8)
project(phragon)

#Bring the headers, such as Student.h into the project
include_directories(includes)

#Can manually add the sources using the set command as follows:
set(SOURCES src/*.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(phragon ${SOURCES})

target_link_libraries( phragon )

