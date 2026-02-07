#pragma once

#include "Math.hpp"
#include "Shader.hpp"
#include <vector>
#include <string>

namespace tori {
    struct Mesh;
    struct Animation;
    struct Node;

    class Model {
    public:
        Model() = default;
        ~Model();

        // Supports GLB files
        static Model* load(const char* path);
        
        // Animation
        void animate(int anim_idx, float time);
        int anim_count() const;
        float anim_duration(int anim_idx) const;

        // Rendering
        void draw(const Mat4& mvp);
        
        // Utils
        Aabb bounds() const;
    private:
        struct Mesh {
            unsigned int vao = 0, vbo = 0, ebo = 0;
            int index_count = 0;
            Vec4 base_color = { 1, 1, 1, 1 };
            Aabb bounds;
        };

        struct Node {
            int parent = -1;
            Mat4 local = Mat4::identity();
            Mat4 global = Mat4::identity();
            Vec3 translation = { 0, 0, 0 };
            Quat rotation = { 0, 0, 0, 1 };
            Vec3 scale = { 1, 1, 1};
        };

        // Data
        std::vector<Mesh> meshes_;
        std::vector<Node> nodes_;
        
        // Skinning Data
        std::vector<Mat4> inverse_binds_;
        std::vector<int> joint_nodes_;
        std::vector<Mat4> joint_matrices_;

        // Animation Data

        struct Channel {
            std::vector<float> times;
            std::vector<float> values;
            int node_index;
            int path; // 0 = trans, 1 = rot, 2 = scale
            int interpolation;
        };

        struct Anim {
            std::vector<Channel> channels;
            float duration = 0.0f;
        };

        std::vector<Anim> animations_;

        Aabb bounds_;
        
        // Resources
        static Shader skin_shader_;
        static void ensure_shader();
    };
}