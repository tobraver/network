cmake_minimum_required(VERSION 3.1.0)

project(network)

# flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# yaml-cpp
file(GLOB yaml-cpp-contrib-sources "third/yaml-cpp/src/contrib/*.cpp")
file(GLOB yaml-cpp-sources "third/yaml-cpp/src/*.cpp")

SET(SRC_LIST 
    network.cpp
    ${yaml-cpp-contrib-sources}
    ${yaml-cpp-sources}
)

# target
add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

# -I
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/third/yaml-cpp/include
    ${CMAKE_CURRENT_LIST_DIR}/third/yaml-cpp/src
)

# -L
target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC )

# -l
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC )

# example
add_subdirectory(example)

