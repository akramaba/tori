#include "../inc/Engine.hpp"
#include "../inc/Camera.hpp"
#include "../inc/Window.hpp"
#include "../lib/miniglad.h"

namespace tori {
    Arena Engine::arena_;
    Scene* Engine::active_scene_ = nullptr;
    
    Scene::Scene() {
        model_octree = std::make_unique<Octree>(Aabb{
            { -10000.0f, -10000.0f, -10000.0f }, 0,
            { 10000.0f, 10000.0f, 10000.0f }, 0
        });
    }

    void Scene::add_model(Model* model, Mat4 transform) {
        if (!model) {
            return;
        }
        
        models.push_back(model);
        transforms.push_back(transform);

        size_t index = models.size() - 1;
        Aabb world_bounds = transform_aabb(transform, model->bounds());
        model_octree->insert((void*)index, world_bounds);
    }

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

        // Frustum Culling via Octree
        Frustum frustum = extract_frustum(vp);
        std::vector<void*> visible_indices = active_scene_->model_octree->query(frustum);
        
        size_t visible_count = visible_indices.size();
        printf("Visible models: %zu\n", visible_count);
        
        // Render visible models w/ any transforms

        RenderCommand* commands = arena_.alloc_array<RenderCommand>(visible_count);

        if (!commands) {
            return;
        }

        // Fill RenderCommands
        for (size_t i = 0; i < visible_count; ++i) {
            size_t idx = (size_t)visible_indices[i];
            Model* model = active_scene_->models[idx];
            
            if (!model) {
                commands[i].model = nullptr;
                continue;
            }

            Mat4 model_matrix = active_scene_->transforms[idx];
            Mat4 mvp = vp * model_matrix;

            commands[i].model = model;
            commands[i].mvp = mvp;
        }

        // Execute RenderCommands
        for (size_t i = 0; i < visible_count; ++i) {
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