#include "../inc/Input.hpp"
#include "../inc/Window.hpp"
#include <cstring> 

// Interesting linker "shenanigans"
extern "C" {
    #define RGFWDEF extern 
    #include <RGFW.h>
}

namespace tori {
    Input::State Input::state_ = {};

    void Input::init() {
        state_ = {};
        state_.first_mouse = true;
        
        int mx;
        int my;
        RGFW_window_getMouse((RGFW_window*)Window::handle(), &mx, &my);

        state_.mouse_pos = {
            (float)mx, 
            (float)my
        };
        state_.prev_mouse_pos = state_.mouse_pos;
    }

    void Input::update() {
        std::memcpy(state_.prev_keys, state_.keys, sizeof(state_.keys));
        std::memcpy(state_.prev_buttons, state_.buttons, sizeof(state_.buttons));
        state_.prev_mouse_pos = state_.mouse_pos;
        state_.scroll = 0;

        RGFW_window* win = (RGFW_window*)Window::handle();
        RGFW_event event;

        while (RGFW_window_checkEvent(win, &event)) {
            switch (event.type) {
                case RGFW_keyPressed:
                    if (event.key.value < MAX_KEYS) {
                        state_.keys[event.key.value] = true;
                    }
                    
                    break;
                case RGFW_keyReleased:
                    if (event.key.value < MAX_KEYS) {
                        state_.keys[event.key.value] = false;
                    }

                    break;
                case RGFW_mouseButtonPressed:
                    if (event.button.value < MAX_BUTTONS) {
                        state_.buttons[event.button.value] = true;
                    }

                    break;
                case RGFW_mouseButtonReleased:
                    if (event.button.value < MAX_BUTTONS) {
                        state_.buttons[event.button.value] = false;
                    }

                    break;
                case RGFW_mouseScroll:
                    state_.scroll = event.scroll.y;
                    break;
                case RGFW_mousePosChanged:
                    state_.mouse_pos.x = (float)event.mouse.x;
                    state_.mouse_pos.y = (float)event.mouse.y;

                    if (state_.first_mouse) {
                        state_.prev_mouse_pos = state_.mouse_pos;
                        state_.first_mouse = false;
                    }

                    break;
                default: 
                    break;
            }
        }
    }

    bool Input::key(Key k) {
        int idx = static_cast<int>(k);
        return idx >= 0 && idx < MAX_KEYS && state_.keys[idx];
    }

    bool Input::key_down(Key k) {
        int idx = static_cast<int>(k);
        return idx >= 0 && idx < MAX_KEYS && state_.keys[idx] && !state_.prev_keys[idx];
    }

    bool Input::key_up(Key k) {
        int idx = static_cast<int>(k);
        return idx >= 0 && idx < MAX_KEYS && !state_.keys[idx] && state_.prev_keys[idx];
    }

    bool Input::mouse_btn(MouseButton b) {
        int idx = static_cast<int>(b);
        return idx >= 0 && idx < MAX_BUTTONS && state_.buttons[idx];
    }

    bool Input::mouse_btn_down(MouseButton b) {
        int idx = static_cast<int>(b);
        return idx >= 0 && idx < MAX_BUTTONS && state_.buttons[idx] && !state_.prev_buttons[idx];
    }

    bool Input::mouse_btn_up(MouseButton b) {
        int idx = static_cast<int>(b);
        return idx >= 0 && idx < MAX_BUTTONS && !state_.buttons[idx] && state_.prev_buttons[idx];
    }

    Vec2 Input::mouse_pos() { 
        return state_.mouse_pos; 
    }
    
    Vec2 Input::mouse_delta() {
        return state_.mouse_pos - state_.prev_mouse_pos;
    }

    float Input::scroll() { 
        return state_.scroll; 
    }

    void Input::set_cursor_locked(bool locked) {
        state_.cursor_locked = locked;
        RGFW_window* win = (RGFW_window*)Window::handle();

        if (locked) {
            RGFW_window_holdMouse(win);
            state_.first_mouse = true;
        } else {
            RGFW_window_unholdMouse(win);
        }
    }
}