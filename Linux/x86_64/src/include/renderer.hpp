#ifndef RENDERER_HPP
#define RENDERER_HPP
// 3rd Party Libraries
#include <SDL2/SDL.h>

// Platform Libraries

// Project Libraries
#include "logger.hpp"

// Standard Libraries
#include <memory>

#ifdef NEO_LSAN_ENABLED
#define NEO_LSAN_DISABLE() __lsan_disable()
#define NEO_LSAN_ENABLE()  __lsan_enable()
#else
// Expand to nothing in non-sanitize builds
#define NEO_LSAN_DISABLE() do {} while(0)
#define NEO_LSAN_ENABLE()  do {} while(0)
#endif

class Renderer {
public:
    // --- Access Singleton Instance ---
    static Renderer & instance() {
        static Renderer instance;
        return instance;
    }

    SDL_Renderer * renderer = nullptr;
    SDL_Window * window = nullptr;

    bool is_SDL_init = false;

    void NEO_SDL_CreateWindow();
    void NEO_SDL_CreateRenderer();

private:
    // --- Private Constructor / Destructor ---
    Renderer();
    ~Renderer();

    // --- Disable copy and move ---
    Renderer(const Renderer &) = delete;
    Renderer & operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer & operator=(Renderer &&) = delete;
};

#endif // RENDERER_HPP
