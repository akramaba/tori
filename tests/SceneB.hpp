#pragma once

#include "BaseScene.hpp"
#include "../inc/Input.hpp"
#include "../inc/Model.hpp"
#include "../inc/Camera.hpp"
#include "../inc/Audio.hpp"

using namespace tori;

class SceneA;
extern SceneA scene_a;

// Tests audio and receiving data from SceneA
class SceneB : public tori::Scene {
public:
    void init() override {
        if (!loaded_) {
            loaded_ = true;
            music_ = Audio::load("audio.mp3");

            Audio::set_loop(music_, true);
        }

        anim_time_ = 0;
        size_ = 5.0f;

        Audio::play(music_);

        // Calculate camera based on the first model handed off from Scene A
        if (!models.empty() && models[0]) {
            Aabb bounds = models[0]->bounds();

            size_ = length(bounds.max - bounds.min);
            if (size_ < 0.01f) {
                size_ = 5.0f;
            }

            Vec3 center = (bounds.min + bounds.max) * 0.5f;
            
            // Sees front and side of model
            camera.position = { size_ * 2.0f, center.y, size_ * 0.5f };
            camera.yaw = 3.14159f;
            camera.pitch = 0;
            camera.fov = 60.0f * DEG2RAD;
        }
    }

    void update() override {
        camera.fly(size_ * 0.5f, 0.002f);

        // Animate all models
        for (auto* m : models) {
            if (m && m->anim_count() > 0) {
                m->animate(0, anim_time_);
            }
        }
        
        anim_time_ += Window::dt();

        if (Input::key_down(Key::Space)) {
            Audio::stop(music_);
            Engine::set_scene((Scene*)&scene_a);
        }
    }

private:
    float anim_time_ = 0.0f;
    bool loaded_ = false;
    float size_ = 5.0f;
    tori::AudioHandle music_;
};