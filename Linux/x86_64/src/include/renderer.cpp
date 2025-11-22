// 3rd Party Libraries
#include <SDL2/SDL.h>

// Platform Libraries

// Project Libraries
#include "logger.hpp"
#include "renderer.hpp"

// Standard Libraries

bool Renderer::init(const char* title, int width, int height) noexcept {
    bool ret = false;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        ERRO(stdout, "SDL_Init() failed: %s", SDL_GetError());
        return false;
    }

    // Create window
    sdl_window.reset(SDL_CreateWindow(title,
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      width,
                                      height,
                                      SDL_WINDOW_SHOWN));
    if (!sdl_window) {
        ERRO(stdout, "SDL_CreateWindow() failed: %s", SDL_GetError());
        SDL_Quit();
        goto SDL_CreateWindow_fail;
    }

    // Create hardware accelerated renderer
    sdl_renderer.reset(SDL_CreateRenderer(sdl_window.get(),
                       -1,
                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!sdl_renderer) {
        ERRO(stdout, "SDL_CreateRenderer() failed: %s", SDL_GetError());
        sdl_window.reset();
        SDL_Quit();
        goto SDL_CreateRenderer_fail;
    }

    ret = true;
    INFO(stdout, "Renderer initialized successfully.");

SDL_CreateRenderer_fail:
    if (!ret) sdl_window.reset();
SDL_CreateWindow_fail:
    if (!ret) SDL_Quit();
    return ret;
}

void Renderer::shutdown() noexcept {
    if (sdl_renderer) {
        INFO(stdout, "Destroying SDL_Renderer...");
        sdl_renderer.reset(nullptr);
    }

    if (sdl_window) {
        INFO(stdout, "Destroying SDL_Window...");
        sdl_window.reset(nullptr);
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    SDL_Quit();
    INFO(stdout, "SDL shutdown completed.");
    return;
}

void Renderer::clear() noexcept {
    if (sdl_renderer) {
        SDL_SetRenderDrawColor(sdl_renderer.get(), 128, 128, 0, 255);
        SDL_RenderClear(sdl_renderer.get());
    }
    return;
}

void Renderer::present() noexcept {
    if (sdl_renderer) {
        SDL_RenderPresent(sdl_renderer.get());
    }
    return;
}
