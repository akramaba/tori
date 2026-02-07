#pragma once

#include "../inc/Engine.hpp"
#include "../inc/Input.hpp"
#include "../inc/Model.hpp"
#include "../inc/Camera.hpp"
#include "../inc/Window.hpp"

using namespace tori;

class SceneB;
extern SceneB scene_b;

// Tests model loading and animation, as well as input handling and scene switching
class SceneA : public tori::Scene {
public:
    void init() override {
        if (!loaded_) {
            loaded_ = true;
            model_ = Model::load("model.glb");
            
            size_ = 5.0f;
            if (model_) {
                Aabb bounds = model_->bounds();
                
                size_ = length(bounds.max - bounds.min);
                if (size_ < 0.01f) {
                    size_ = 5.0f;
                }
            }
        }

        // Reset just in case coming back from Scene B
        models.clear();
        transforms.clear();

        ambient_color = { 0.1f, 0.1f, 0.15f };
        sun_dir = normalize(Vec3{ 0.5f, 1.0f, 0.3f });
        sun_color = { 1.0f, 0.95f, 0.9f };

        if (model_) {
            add_model(model_, Mat4::identity());
        }

        anim_time_ = 0;

        if (model_) {
            Aabb bounds = model_->bounds();
            Vec3 center = (bounds.min + bounds.max) * 0.5f;
            
            camera.position = { size_ * 2.0f, center.y, size_ * 0.5f };
            camera.yaw = 3.14159f;
            camera.pitch = 0;
            camera.fov = 60.0f * DEG2RAD;
            camera.update();
        }
    }

    void update() override {
        camera.fly(size_ * 0.5f, 0.002f);

        if (model_ && model_->anim_count() > 0) {
            anim_time_ += Window::dt();
            model_->animate(0, anim_time_);
        }

        // Switch Logic
        if (Input::key_down(Key::Space)) {
            // Hand-off models to Scene B
            copy_models_to_b(); 
            Engine::set_scene((Scene*)&scene_b);
        }
    }

    void copy_models_to_b();
private:
    tori::Model* model_ = nullptr;
    float anim_time_ = 0.0f;
    bool loaded_ = false;
    float size_ = 5.0f;
};