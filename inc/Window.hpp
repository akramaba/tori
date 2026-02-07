#pragma once

namespace tori {
    class Window {
    public:
        static bool init(int width, int height, const char* title);
        static void poll();
        static void swap();
        static bool should_close();
        static void close();
        
        // Accessors
        static int width();
        static int height();
        static float dt(); // Delta time
        static float time();

        // Input system needs handle for cursor
        static void* handle();
    private:
        struct State {
            void* handle = nullptr;
            int width = 0;
            int height = 0;
            float delta_time = 0.0f;
            double last_time = 0.0;
        };

        static State state_;
    };
}