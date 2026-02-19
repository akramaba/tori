#include "../inc/Engine.hpp"
#include "../inc/Camera.hpp"
#include "../inc/Window.hpp"
#include "../lib/miniglad.h"

namespace tori {
    Arena Engine::arena_;
    Scene* Engine::active_scene_ = nullptr;

    bool Engine::init() {
        arena_.init(10 * 1024 * 1024);
        return true;
    }

    void Engine::quit() {
        // TODO: Any proper cleanup needed?
        active_scene_ = nullptr;
    }

    // Chains for parent-child scene relationships (parents init/update before children)

    static void call_init_chain(Scene* scene) {
        if (!scene) {
            return;
        }

        if (scene->parent) {
            call_init_chain(scene->parent);
        }
        
        scene->init();
    }

    static void call_update_chain(Scene* scene) {
        if (!scene) {
            return;
        }

        if (scene->parent) {
            call_update_chain(scene->parent);
        }

        scene->update();
    }

    void Engine::set_scene(Scene* scene) {
        active_scene_ = scene;

        if (scene) {
            call_init_chain(scene);
        }
    }

    void Engine::render() {
        if (active_scene_) {
            const Vec3& cc = active_scene_->clear_color;
            glClearColor(cc.x, cc.y, cc.z, 1.0f);
        } else {
            // TODO: Variable for default color?
            glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!active_scene_) {
            return;
        }

        arena_.reset();

        Mat4 view = camera.view();
        Mat4 proj = camera.proj();
        Mat4 vp = proj * view;

        size_t count = active_scene_->models.size();
        size_t tf_count = active_scene_->transforms.size();

        // Render all models in the scene w/ any transforms

        // Allocate RenderCommands from Arena
        RenderCommand* commands = arena_.alloc_array<RenderCommand>(count);

        if (!commands) {
            return;
        }

        // Fill RenderCommands
        for (size_t i = 0; i < count; ++i) {
            Model* model = active_scene_->models[i];
            if (!model) {
                commands[i].model = nullptr;
                continue;
            }

            Mat4 model_matrix = (i < tf_count) ? active_scene_->transforms[i] : Mat4::identity();
            Mat4 mvp = vp * model_matrix;

            commands[i].model = model;
            commands[i].mvp = mvp;
        }

        // Execute RenderCommands
        for (size_t i = 0; i < count; ++i) {
            if (commands[i].model) {
                commands[i].model->draw(commands[i].mvp);
            }
        }

        call_update_chain(active_scene_);
    }

    Scene* Engine::current_scene() {
        return active_scene_;
    }

    Arena& Engine::arena() {
        return arena_;
    }
}