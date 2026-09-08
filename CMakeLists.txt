cmake_minimum_required(VERSION 3.16)
project(GraficaProgetto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML 3.0
find_package(SFML 3.0 CONFIG REQUIRED COMPONENTS Graphics Window System)

# For each tappa, add an executable
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

add_executable(Tappa06 Tappa06/main.cpp Tappa06/pvz.cpp)
target_link_libraries(Tappa06 PRIVATE SFML::Graphics)

add_executable(Tappa07 Tappa07/main.cpp Tappa07/pvz.cpp)
target_link_libraries(Tappa07 PRIVATE SFML::Graphics)

add_executable(Tappa08 Tappa08/main.cpp Tappa08/pvz.cpp)
target_link_libraries(Tappa08 PRIVATE SFML::Graphics)

add_executable(Tappa09 Tappa09/main.cpp Tappa09/pvz.cpp)
target_link_libraries(Tappa09 PRIVATE SFML::Graphics)

add_executable(Tappa10 Tappa10/main.cpp Tappa10/pvz.cpp)
target_link_libraries(Tappa10 PRIVATE SFML::Graphics)