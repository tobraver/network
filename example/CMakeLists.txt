cmake_minimum_required(VERSION 3.1.0)

project(network_example)

# target
add_executable(network_example example.cpp)

# -l
target_link_libraries(network_example PRIVATE network)