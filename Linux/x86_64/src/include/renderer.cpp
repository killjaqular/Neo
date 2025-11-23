// 3rd Party Libraries
#ifdef NEO_LSAN_ENABLED
#include <sanitizer/lsan_interface.h>
#endif
#include <SDL2/SDL.h>

// Platform Libraries

// Project Libraries
#include "logger.hpp"
#include "renderer.hpp"

// Standard Libraries

Renderer::Renderer() {
    is_SDL_init = true;
    NEO_LSAN_DISABLE();
    INFO(stdout, "SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)");
    if (SDL_Init(SDL_INIT_VIDEO |
                 SDL_INIT_AUDIO |
                 SDL_INIT_TIMER)) {
        is_SDL_init = false;
        ERRO(stdout, "SDL_Init() fail: %s", SDL_GetError());
    }
    NEO_LSAN_ENABLE();
    return;
}

void Renderer::NEO_SDL_CreateWindow() {
    NEO_LSAN_DISABLE();
    window = SDL_CreateWindow("Neo",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              512,
                              512,
                              SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
    NEO_LSAN_ENABLE();
    if (!window) {
        ERRO(stdout, "SDL_CreateWindow() fail: %s", SDL_GetError());
    }
    return;
}

void Renderer::NEO_SDL_CreateRenderer() {
    if (!window) {
        return;
    } else {
        NEO_LSAN_DISABLE();
        renderer = SDL_CreateRenderer(window,
                                      0,
                                      SDL_RENDERER_ACCELERATED);
        NEO_LSAN_ENABLE();
        if (!renderer) {
            ERRO(stdout, "SDL_CreateRenderer() fail: %s", SDL_GetError());
        }
    }
    return;
}

Renderer::~Renderer() {
    if (renderer) SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    if (window) SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
    INFO(stdout, "SDL_Quit()");
    is_SDL_init = false;
    return;
}
