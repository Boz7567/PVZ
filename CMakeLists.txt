cmake_minimum_required(VERSION 3.16)
project(PVZ)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(Tappa01 Tappa01/main.cpp)
target_link_libraries(Tappa01 PRIVATE SFML::Graphics)

add_executable(Tappa02 Tappa02/main.cpp)
target_link_libraries(Tappa02 PRIVATE SFML::Graphics)

add_executable(Tappa03 Tappa03/main.cpp)
target_link_libraries(Tappa03 PRIVATE SFML::Graphics)

add_executable(Tappa04 Tappa04/main.cpp)
target_link_libraries(Tappa04 PRIVATE SFML::Graphics)

add_executable(Tappa05 Tappa05/main.cpp Tappa05/pvz.cpp)
target_link_libraries(Tappa05 PRIVATE SFML::Graphics)
