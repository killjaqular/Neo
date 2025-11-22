#ifndef RENDERER_HPP
#define RENDERER_HPP

// 3rd Party Libraries
#include <SDL2/SDL.h>

// Project Libraries
#include "logger.hpp"

// Standard Libraries
#include <memory>

class Renderer {
public:
    // --- Access Singleton Instance ---
    static Renderer & instance() {
        static Renderer instance;
        return instance;
    }

    // --- Initialize SDL + create window + renderer ---
    bool init(const char * title, int width, int height) noexcept;

    // --- Shutdown SDL resources safely ---
    void shutdown() noexcept;

    // --- Drawing helpers ---
    void clear() noexcept;
    void present() noexcept;

private:
    struct WindowDeleter {
        void operator()(SDL_Window * w) const noexcept {
            if (w) SDL_DestroyWindow(w);
        }
    };

    struct RendererDeleter {
        void operator()(SDL_Renderer * r) const noexcept {
            if (r) SDL_DestroyRenderer(r);
        }
    };

    using WindowPtr   = std::unique_ptr<SDL_Window, WindowDeleter>;
    using RendererPtr = std::unique_ptr<SDL_Renderer, RendererDeleter>;

    WindowPtr   sdl_window   = nullptr;
    RendererPtr sdl_renderer = nullptr;

    // --- Private Constructor / Destructor ---
    Renderer() = default;
    ~Renderer() = default;

    // --- Disable copy and move ---
    Renderer(const Renderer &) = delete;
    Renderer & operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer & operator=(Renderer &&) = delete;
};

#endif // RENDERER_HPP
