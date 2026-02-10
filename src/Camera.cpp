#include "../inc/Camera.hpp"
#include "../inc/Window.hpp"
#include "../inc/Input.hpp"
#include <cmath>

namespace tori {
    // Global camera
    Camera camera;

    void Camera::init() {
        position = { 0, 0, 5 };
        pitch = 0;
        yaw = -90.0f * DEG2RAD;
        fov = 60.0f * DEG2RAD;
        near_clip = 0.1f;
        far_clip = 1000.0f;
    }

    [[gnu::pure]]
    Vec3 Camera::forward() const noexcept {
        return forward_;
    }

    [[gnu::pure]]
    Vec3 Camera::right() const noexcept {
        return right_;
    }

    void Camera::update_vectors() {
        float sp = std::sin(pitch);
        float cp = std::cos(pitch);
        float sy = std::sin(yaw);
        float cy = std::cos(yaw);

        // Saving vectors is faster than recomputing trig every frame

        forward_ = {
            cy * cp,
            sp,
            sy * cp
        };

        right_ = { 
            -sy, 
            0.0f, 
            cy 
        };

        up_ = cross(right_, forward_);
    }

    [[gnu::hot]]
    void Camera::update() {
        Vec3 target = position + forward_;
        Vec3 world_up = { 
            0,
            1,
            0 
        };
        
        view_ = look_at(position, target, world_up);

        float aspect = (float)Window::width() / (float)Window::height();

        // If the window is minimized, potential divide by zero
        if (Window::height() == 0) [[unlikely]] {
            aspect = 1.0f;
        }

        proj_ = perspective(fov, aspect, near_clip, far_clip);

        // Update frustum for culling
        Mat4 vp = proj_ * view_;
        frustum_ = extract_frustum(vp);
    }

    [[gnu::hot]]
    void Camera::fly(float speed, float sensitivity) {
        // TODO: Not use lazy init
        static bool first_time = true;
        if (first_time) [[unlikely]] {
            first_time = false;
            update_vectors();
        }

        float dt = Window::dt();
        float move_speed = speed * dt;

        if (Input::key(Key::Shift)) [[unlikely]] {
            move_speed *= 2.5f;
        }

        if (Input::key(Key::W)) {
            position = position + (forward_ * move_speed);
        }

        if (Input::key(Key::S)) {
            position = position - (forward_ * move_speed);
        }

        if (Input::key(Key::D)) {
            position = position + (right_ * move_speed);
        }

        if (Input::key(Key::A)) {
            position = position - (right_ * move_speed);
        }

        if (Input::key(Key::E)) {
            position.y += move_speed;
        }

        if (Input::key(Key::Q)) {
            position.y -= move_speed;
        }

        // Zoom
        float scroll_delta = Input::scroll();
        if (scroll_delta != 0) [[unlikely]] {
            fov -= scroll_delta * 2.0f * DEG2RAD;
            
            if (fov < 10.0f * DEG2RAD) {
                fov = 10.0f * DEG2RAD;
            }

            if (fov > 120.0f * DEG2RAD) {
                fov = 120.0f * DEG2RAD;
            }
        }

        update();
    }

    [[gnu::pure]]
    bool Camera::cull(const Aabb& box) const noexcept {
        return cull_aabb(frustum_, box);
    }

    [[gnu::pure]]
    const Mat4& Camera::view() const noexcept {
        return view_;
    }

    [[gnu::pure]]
    const Mat4& Camera::proj() const noexcept {
        return proj_;
    }
}