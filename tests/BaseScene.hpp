#pragma once

#include "../inc/Engine.hpp"
#include "../inc/Window.hpp"
#include <cmath>

using namespace tori;

// Tests the basic scene setup and parent-child relationships (chain logic)
class BaseScene : public tori::Scene {
public:
    void init() override {
        if (!loaded_) {
            loaded_ = true;
        }

        time_ = 0.0f;
        ambient_color = { 0.1f, 0.1f, 0.15f };
        sun_dir = tori::normalize(Vec3{ 0.5f, 1.0f, 0.3f });
        sun_color = { 1.0f, 0.95f, 0.9f };
    }

    void update() override {
        time_ += tori::Window::dt();
    }
private:
    bool loaded_ = false;
    float time_ = 0.0f;
};