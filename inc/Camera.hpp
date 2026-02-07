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
        bool cull(const Aabb& box) const;
        Vec3 forward() const;
        Vec3 right() const;

        // Matrix
        const Mat4& view() const;
        const Mat4& proj() const;
    private:
        Mat4 view_;
        Mat4 proj_;
        Frustum frustum_;
    };

    // Global camera
    extern Camera camera;
}