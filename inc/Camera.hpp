#pragma once

#include "Math.hpp"

namespace tori {
    class Camera {
    public:
        // TODO: Consider getters/setters instead of public fields
        Vec3 position;
        float pitch;
        float yaw;
        float fov;
        float near_clip;
        float far_clip;

        void init();
        void update();
        
        // Free camera
        void fly(float speed, float sensitivity);
        
        // Utils
        [[gnu::pure]] bool cull(const Aabb& box) const noexcept;
        [[gnu::pure]] Vec3 forward() const noexcept;
        [[gnu::pure]] Vec3 right() const noexcept;

        // Matrix
        [[gnu::pure]] const Mat4& view() const noexcept;
        [[gnu::pure]] const Mat4& proj() const noexcept;
    private:
        void update_vectors();

        Vec3 forward_;
        Vec3 right_;
        Vec3 up_;

        Mat4 view_;
        Mat4 proj_;
        Frustum frustum_;
    };

    // Global camera
    extern Camera camera;
}