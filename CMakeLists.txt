cmake_minimum_required(VERSION 3.16)
project(GraficaProgetto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(Tappa01 Tappa01/main.cpp)
target_link_libraries(Tappa01 PRIVATE SFML::Graphics)
