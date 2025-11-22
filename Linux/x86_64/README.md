# Neo

## Build Debug
Neo/Linux/x86_64>cmake -S . -B build/build_debug
Neo/Linux/x86_64>cmake --build build/build_debug

## Build Sanitizer
Neo/Linux/x86_64>cmake -S . -B build/build_sanitize -DCMAKE_BUILD_TYPE=Sanitize
Neo/Linux/x86_64>cmake --build build/build_sanitize

## Build Release
Neo/Linux/x86_64>cmake -S . -B build/build_release -DCMAKE_BUILD_TYPE=Release
Neo/Linux/x86_64>cmake --build build/build_release

## TODO: I need to actually write the fuzzer
# Build Fuzzer
Neo/Linux/x86_64>cmake -S . -B build/build_fuzz -DCMAKE_BUILD_TYPE=Fuzz
Neo/Linux/x86_64>cmake --build build/build_fuzz

# Requirements
SDL2 `https://wiki.libsdl.org/SDL2/FrontPage`
C++20 Compiler
