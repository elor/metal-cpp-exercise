cmake_minimum_required(VERSION 3.0)
project(MetalStuff)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp)

add_custom_target(add.air COMMAND xcrun -sdk macosx metal -c ${CMAKE_SOURCE_DIR}/add.metal -o ${CMAKE_SOURCE_DIR}/add.air SOURCES add.metal)

add_custom_target(main.metallib COMMAND xcrun -sdk macosx metallib ${CMAKE_SOURCE_DIR}/add.air -o ${CMAKE_SOURCE_DIR}/main.metallib)
add_dependencies(main.metallib add.air)

add_executable(main main.cpp definitions.cpp)
add_dependencies(main main.metallib)
target_link_libraries(main "-framework Metal" "-framework QuartzCore" "-framework Foundation")
