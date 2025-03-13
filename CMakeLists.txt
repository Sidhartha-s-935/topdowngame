cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include directories
include_directories(${SFML_INCLUDE_DIR})

# Source files
set(SOURCES
    src/main.cpp
    src/player.cpp
)

# Add executable
add_executable(Game ${SOURCES})

# Link SFML libraries
target_link_libraries(Game sfml-graphics sfml-window sfml-system sfml-audio)

