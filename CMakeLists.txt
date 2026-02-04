cmake_minimum_required(VERSION 3.16)
project(TowerDefenseGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

file(GLOB SRC_FILES src/*.cpp)

add_executable(TowerDefenseGame ${SRC_FILES})
target_include_directories(TowerDefenseGame PRIVATE src)

target_link_libraries(TowerDefenseGame PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)
