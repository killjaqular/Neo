cmake_minimum_required(VERSION 3.16)

# Default compiler (Clang 18)
if(NOT DEFINED CMAKE_C_COMPILER AND NOT DEFINED ENV{CC})
    set(CMAKE_C_COMPILER clang-18)
endif()

if(NOT DEFINED CMAKE_CXX_COMPILER AND NOT DEFINED ENV{CXX})
    set(CMAKE_CXX_COMPILER clang++-18)
endif()

project(Neo LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Supported build types
set(VALID_BUILD_TYPES Debug Release Sanitize Fuzz)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose build type" FORCE)
endif()

if(NOT CMAKE_BUILD_TYPE IN_LIST VALID_BUILD_TYPES)
    message(FATAL_ERROR
        "Invalid build type '${CMAKE_BUILD_TYPE}'. "
        "Valid build types: Debug, Release, Sanitize, Fuzz"
    )
endif()

message(STATUS "Using build type: ${CMAKE_BUILD_TYPE}")

# Build-type-specific flags
set(COMMON_WARNINGS -Wall -Wextra -Wpedantic)

# Flags
set(DEBUG_FLAGS     -g -O0 -DLOG)
set(RELEASE_FLAGS   -O3)
set(SANITIZE_FLAGS  -g -O0 -DLOG
                     -fsanitize=address,undefined,leak
                     -fno-omit-frame-pointer)
set(FUZZ_FLAGS      -g -O1
                     -fsanitize=fuzzer,address,undefined
                     -fno-omit-frame-pointer)

# SDL2
find_package(SDL2 REQUIRED CONFIG)

# Source Files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# Final Binary
add_executable(${PROJECT_NAME} ${SOURCES})

# Include dirs (target-scoped, not global)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        src/include
)

# Link SDL2
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2
)

# Build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE ${DEBUG_FLAGS} ${COMMON_WARNINGS})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE ${RELEASE_FLAGS} ${COMMON_WARNINGS})
elseif(CMAKE_BUILD_TYPE STREQUAL "Sanitize")
    target_compile_options(${PROJECT_NAME} PRIVATE ${SANITIZE_FLAGS} ${COMMON_WARNINGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${SANITIZE_FLAGS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE NEO_LSAN_ENABLED)
elseif(CMAKE_BUILD_TYPE STREQUAL "Fuzz")
    target_compile_options(${PROJECT_NAME} PRIVATE ${FUZZ_FLAGS} ${COMMON_WARNINGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${FUZZ_FLAGS})
endif()
