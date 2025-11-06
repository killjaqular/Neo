cmake_minimum_required(VERSION 3.16)
project(Neo LANGUAGES CXX)

# --- C++20 Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER "clang++")

# --- Default Build Type ---
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# --- Custom Flags ---
set(CMAKE_CXX_FLAGS_DEBUG     "-g -O0 -DLOG")
set(CMAKE_CXX_FLAGS_RELEASE   "-O3")
set(CMAKE_CXX_FLAGS_SANITIZE  "-g -O0 -DLOG -fsanitize=address,undefined,leak -fno-omit-frame-pointer")

# --- Common Compiler Flags ---
add_compile_options(-Wall -Wextra -Wpedantic)

# --- Sources ---
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/include/*.cpp"
)

# --- Include Directories ---
include_directories(src/include)

# --- Target ---
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src/include)
