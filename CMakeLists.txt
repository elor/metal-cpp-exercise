cmake_minimum_required(VERSION 3.0)
project(MetalStuff)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp)

add_executable(main main.cpp definitions.cpp)
target_link_libraries(main "-framework Metal" "-framework QuartzCore" "-framework Foundation")
