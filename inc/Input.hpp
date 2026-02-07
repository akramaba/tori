#pragma once

#include "Math.hpp"
#include <cstdint>
    
namespace tori {
    // TODO: Add more RGFW keys
    enum class Key : int {
        W = 'w',
        A = 'a',
        S = 's',
        D = 'd',
        Q = 'q',
        E = 'e',
        Space = ' ',
        Escape = '\033',
        Shift = 0x81
    };

    enum class MouseButton : int {
        Left = 0,
        Middle = 1,
        Right = 2
    };

    class Input {
    public:
        static void init();
        static void update();

        // Keyboard
        static bool key(Key k);
        static bool key_down(Key k);
        static bool key_up(Key k);

        // Mouse
        static bool mouse_btn(MouseButton b);
        static bool mouse_btn_down(MouseButton b);
        static bool mouse_btn_up(MouseButton b);
        
        static Vec2 mouse_pos();
        static Vec2 mouse_delta();
        static float scroll();
        
        static void set_cursor_locked(bool locked);
    private:
        static constexpr int MAX_KEYS = 256;
        static constexpr int MAX_BUTTONS = 8;

        struct State {
            bool keys[MAX_KEYS];
            bool prev_keys[MAX_KEYS];
            bool buttons[MAX_BUTTONS];
            bool prev_buttons[MAX_BUTTONS];
            Vec2 mouse_pos;
            Vec2 prev_mouse_pos;
            float scroll;
            bool cursor_locked;
            bool first_mouse;
        };

        static State state_;
    };
}