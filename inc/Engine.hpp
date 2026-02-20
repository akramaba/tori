#pragma once

#include "Math.hpp"
#include "Model.hpp"
#include "Arena.hpp"
#include "Octree.hpp"
#include <vector>

namespace tori {
    class Scene {
    public:
        Scene();
        virtual ~Scene() = default;

        virtual void init() {}
        virtual void update() {}

        Scene* parent = nullptr;

        std::vector<Model*> models;
        std::vector<Mat4> transforms;
        
        // Scenes can have their own lighting
        Vec3 clear_color = { 0.1f, 0.1f, 0.12f };
        Vec3 ambient_color = { 0.0f, 0.0f, 0.0f };
        Vec3 sun_dir = { 0.0f, 1.0f, 0.0f };
        Vec3 sun_color = { 1.0f, 1.0f, 1.0f };

        std::unique_ptr<Octree> model_octree;

        void add_model(Model* model, Mat4 transform = Mat4::identity());
    };

    struct RenderCommand {
        Model* model;
        Mat4 mvp;
    };

    class Engine {
    public:
        static bool init();
        static void quit();
        
        static void set_scene(Scene* scene);
        static void render();

        static Scene* current_scene();
        
        static Arena& arena();
    private:
        static Scene* active_scene_;
        static Arena arena_;
    };
}