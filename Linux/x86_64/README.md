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

## KNOWN MEMORY LEAKS

Seems like there are libraries outside of my control that are not cleaning up resources at
main-exit. We will have the sanitizer ignore these leaks; we cannot do anything about these.

```bash
=================================================================
==57815==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 65536 byte(s) in 1 object(s) allocated from:
    #0 0x5c1ee75ba0a2 in aligned_alloc (/home/ado/dev/CNO/Neo/Linux/x86_64/build/build_sanitize/Neo+0xd20a2) (BuildId: a2b395acb954b9fc00ea6f20dc874d5c8f65d4f5)
    #1 0x73ed9bdcd1df  (<unknown module>)

Direct leak of 512 byte(s) in 1 object(s) allocated from:
    #0 0x5c1ee75b975f in malloc (/home/ado/dev/CNO/Neo/Linux/x86_64/build/build_sanitize/Neo+0xd175f) (BuildId: a2b395acb954b9fc00ea6f20dc874d5c8f65d4f5)
    #1 0x73ed97ac4dd4  (<unknown module>)

Direct leak of 128 byte(s) in 1 object(s) allocated from:
    #0 0x5c1ee75b9b4c in realloc (/home/ado/dev/CNO/Neo/Linux/x86_64/build/build_sanitize/Neo+0xd1b4c) (BuildId: a2b395acb954b9fc00ea6f20dc874d5c8f65d4f5)
    #1 0x73eda52cb7a8  (<unknown module>)

Direct leak of 48 byte(s) in 1 object(s) allocated from:
    #0 0x5c1ee75b9929 in calloc (/home/ado/dev/CNO/Neo/Linux/x86_64/build/build_sanitize/Neo+0xd1929) (BuildId: a2b395acb954b9fc00ea6f20dc874d5c8f65d4f5)
    #1 0x73edaaa91e01 in pthread_cond_broadcast nptl/old_pthread_cond_broadcast.c:33:36

Indirect leak of 608 byte(s) in 4 object(s) allocated from:
    #0 0x5c1ee75b975f in malloc (/home/ado/dev/CNO/Neo/Linux/x86_64/build/build_sanitize/Neo+0xd175f) (BuildId: a2b395acb954b9fc00ea6f20dc874d5c8f65d4f5)
    #1 0x73eda52c7d44  (<unknown module>)

SUMMARY: AddressSanitizer: 66832 byte(s) leaked in 8 allocation(s).
```
