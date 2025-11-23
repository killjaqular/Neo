// 3rd Party Libraries
#include <SDL2/SDL.h>

// Platform Libraries

// Project Libraries
#include "logger.hpp"
#include "renderer.hpp"

// Standard Libraries

Renderer::Renderer() {
    is_SDL_init = true;
    INFO(stdout, "SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)");
    if (SDL_Init(SDL_INIT_VIDEO |
                 SDL_INIT_AUDIO |
                 SDL_INIT_TIMER)) {
        is_SDL_init = false;
        ERRO(stdout, "SDL_Init() fail: %s", SDL_GetError());
        SDL_Log("SDL_Init() fail: %s\n", SDL_GetError());
    }

    return;
}

Renderer::~Renderer() {
    SDL_Quit();
    INFO(stdout, "SDL_Quit()");
    is_SDL_init = false;

    return;
}
