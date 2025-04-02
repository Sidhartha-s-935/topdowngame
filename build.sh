#!/bin/bash

# Set the build directory
BUILD_DIR="build"

# Set the path to the vcpkg toolchain file
# VCPKG_TOOLCHAIN_FILE="/home/nayeem/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Create the build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi

# Run CMake to generate build files with vcpkg toolchain
cmake -B "$BUILD_DIR" -G "Unix Makefiles"  

# Check if CMake succeeded
if [ $? -ne 0 ]; then
  echo "CMake configuration failed."
  exit 1
fi

# Build the project
cmake --build "$BUILD_DIR"

# Check if the build succeeded
if [ $? -ne 0 ]; then
  echo "Build failed."
  exit 1
fi

# Run the executable
"./$BUILD_DIR/Game"
