cmake_minimum_required(VERSION 3.16)
project(Neo LANGUAGES CXX)

# Force C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set Clang++ explicitly
set(CMAKE_CXX_COMPILER "clang++")

# --- Define custom build types ---
# Defaults to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Add "Sanitize" build type
set(CMAKE_CXX_FLAGS_SANITIZE "-g -O0 -DLOG -fsanitize=address,undefined,leak -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_SANITIZE "-fsanitize=address,undefined,leak")

# Debug build (no sanitizer)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DLOG")
set(CMAKE_LINKER_FLAGS_DEBUG "")

# Release build
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_LINKER_FLAGS_RELEASE "")

# Common compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Include directories
include_directories(src/include)

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp)
