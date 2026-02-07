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

        update();
    }

    Vec3 Camera::forward() const {
        return {
            std::cos(yaw) * std::cos(pitch),
            std::sin(pitch),
            std::sin(yaw) * std::cos(pitch)
        };
    }

    Vec3 Camera::right() const {
        return normalize(cross(forward(), { 0, 1, 0 }));
    }

    void Camera::update() {
        Vec3 fwd = forward();
        Vec3 target = position + fwd;
        Vec3 up = { 0, 1, 0 };

        view_ = look_at(position, target, up);

        float aspect = (float)Window::width() / (float)Window::height();
        // If the window is minimized, potential divide by zero
        if (Window::height() == 0) aspect = 1.0f;

        proj_ = perspective(fov, aspect, near_clip, far_clip);

        // Update frustum for culling
        Mat4 vp = proj_ * view_;
        frustum_ = extract_frustum(vp);
    }

    void Camera::fly(float speed, float sensitivity) {
        // Mouse look (hold RMB)
        if (Input::mouse_btn(MouseButton::Right)) {
            Input::set_cursor_locked(true);
            
            Vec2 delta = Input::mouse_delta();
            
            yaw += delta.x * sensitivity;
            pitch -= delta.y * sensitivity;

            // Clamp pitch

            float max_pitch = 89.0f * DEG2RAD;

            if (pitch > max_pitch) {
                pitch = max_pitch;
            }

            if (pitch < -max_pitch) {
                pitch = -max_pitch;
            }
        } else {
            Input::set_cursor_locked(false);
        }

        // Keyboard movement

        Vec3 fwd = forward();
        Vec3 rgt = right();
        float dt = Window::dt();
        float move_speed = speed * dt;

        if (Input::key(Key::Shift)) {
            move_speed *= 2.5f;
        }

        if (Input::key(Key::W)) {
            position = position + (fwd * move_speed);
        }

        if (Input::key(Key::S)) {
            position = position - (fwd * move_speed);
        }

        if (Input::key(Key::D)) {
            position = position + (rgt * move_speed);
        }

        if (Input::key(Key::A)) {
            position = position - (rgt * move_speed);
        }

        if (Input::key(Key::E)) {
            position.y += move_speed;
        }

        if (Input::key(Key::Q)) {
            position.y -= move_speed;
        }

        // Zoom
        float scroll_delta = Input::scroll();
        if (scroll_delta != 0) {
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

    bool Camera::cull(const Aabb& box) const {
        return cull_aabb(frustum_, box);
    }

    const Mat4& Camera::view() const {
        return view_;
    }

    const Mat4& Camera::proj() const {
        return proj_;
    }
}