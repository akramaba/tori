#include "../inc/Window.hpp"

#define RGFW_NO_PASSTHROUGH
#define RGFW_NO_LOAD_WGL
#define RGFW_NO_DPI
#define RGFW_NO_WINMM
#define RGFW_NO_DWM
#define RGFW_IMPLEMENTATION
#define MINIGLAD_IMPLEMENTATION

#include "../lib/miniglad.h"

// More "shenanigans" from linker
extern "C" {
    #define RGFW_IMPLEMENTATION
    #define RGFW_OPENGL
    #define RGFWDEF
    #include "../lib/RGFW.h"
}

#include <ctime>

namespace tori {
    Window::State Window::state_ = {};
    static std::clock_t start_clock = 0;

    bool Window::init(int width, int height, const char* title) {
        start_clock = std::clock();

        RGFW_window* win = RGFW_createWindow(
            title,
            0,
            0,
            width,
            height,
            RGFW_windowCenter | RGFW_windowOpenGL
        );

        if (!win) {
            return false;
        }

        if (!gladLoadGL()) {
            RGFW_window_close(win);
            return false;
        }

        state_.handle = win;
        state_.width = width;
        state_.height = height;
        state_.last_time = time();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        return true;
    }

    void Window::poll() {
        RGFW_window* win = static_cast<RGFW_window*>(state_.handle);
        RGFW_event ev;

        while (RGFW_window_checkEvent(win, &ev)) {
            if (ev.type == RGFW_windowResized) [[unlikely]] {
                int w;
                int h;

                RGFW_window_getSize(win, &state_.width, &state_.height);

                glViewport(0, 0, state_.width, state_.height);
            }
        }

        float now = time();
        state_.delta_time = now - static_cast<float>(state_.last_time);
        state_.last_time = now;
    }

    void Window::swap() {
        RGFW_window_swapBuffers_OpenGL((RGFW_window*)state_.handle);
    }

    bool Window::should_close() {
        if (!state_.handle) [[unlikely]] {
            return true;
        }

        return RGFW_window_shouldClose((RGFW_window*)state_.handle);
    }

    void Window::close() {
        if (state_.handle) [[unlikely]] {
            RGFW_window_close((RGFW_window*)state_.handle);
            state_.handle = nullptr;
        }
    }

    [[gnu::pure]]
    int Window::width() noexcept {
        return state_.width;
    }

    [[gnu::pure]]
    int Window::height() noexcept {
        return state_.height;
    }

    [[gnu::pure]]
    float Window::dt() noexcept {
        return state_.delta_time;
    }

    [[gnu::pure]]
    float Window::time() noexcept {
        return static_cast<float>(std::clock() - start_clock) / CLOCKS_PER_SEC;
    }

    [[gnu::pure]]
    void* Window::handle() noexcept {
        return state_.handle;
    }
}