// TODO: Needs lots of optimization for speed, will look into SIMD and different techniques
// TODO: Also needs more comments and better organization while I still learn it all

#pragma once

#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

namespace tori {
    // Constants
    constexpr float PI = 3.14159265358979323846f;
    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;

    // Types

    struct Vec2 {
        float x;
        float y;
    };

    struct [[gnu::aligned(16)]] Vec3 {
        float x;
        float y;
        float z;
        float _pad;
    };

    struct [[gnu::aligned(16)]] Vec4 {
        float x;
        float y;
        float z;
        float w;
    };

    struct [[gnu::aligned(16)]] Quat {
        float x;
        float y;
        float z;
        float w;

        static constexpr Quat identity() noexcept { 
            return {
                0, 0, 0, 1
            }; 
        }
    };

    struct [[gnu::aligned(16)]] Mat4 {
        float m[16];

        static constexpr Mat4 identity() noexcept {
            return {{ 
                1, 0, 0, 0, 
                0, 1, 0, 0, 
                0, 0, 1, 0, 
                0, 0, 0, 1 
            }};
        }
    };

    struct [[gnu::aligned(16)]] Aabb {
        Vec3 min;
        float _pad1;
        Vec3 max;
        float _pad2;
    };

    // Probably won't be optimized. There's only one
    // frustrum. Contextually, the overhead is negligible.
    struct Frustum {
        Vec4 planes[6];
    };

    // Vec2

    [[gnu::always_inline]] [[gnu::const]]
    inline constexpr Vec2 operator+(Vec2 a, Vec2 b) noexcept { 
        return {
            a.x + b.x, 
            a.y + b.y
        }; 
    }

    [[gnu::always_inline]] [[gnu::const]]
    inline constexpr Vec2 operator-(Vec2 a, Vec2 b) noexcept { 
        return {
            a.x - b.x, 
            a.y - b.y
        }; 
    }

    // Vec3

    [[gnu::always_inline]] [[gnu::const]]
    inline constexpr Vec3 operator+(Vec3 a, Vec3 b) noexcept { 
        return {
            a.x + b.x, 
            a.y + b.y, 
            a.z + b.z
        }; 
    }

    [[gnu::always_inline]] [[gnu::const]]
    inline constexpr Vec3 operator-(Vec3 a, Vec3 b) noexcept { 
        return {
            a.x - b.x, 
            a.y - b.y, 
            a.z - b.z
        }; 
    }

    [[gnu::always_inline]] [[gnu::const]]
    inline constexpr Vec3 operator*(Vec3 v, float s) noexcept { 
        return {
            v.x * s, 
            v.y * s, 
            v.z * s
        }; 
    }

    // Math

    [[gnu::const]]
    inline constexpr float dot(Vec3 a, Vec3 b) noexcept { 
        return a.x * b.x + a.y * b.y + a.z * b.z; 
    }

    [[gnu::const]]
    inline constexpr Vec3 cross(Vec3 a, Vec3 b) noexcept {
        return {
            a.y * b.z - a.z * b.y, 
            a.z * b.x - a.x * b.z, 
            a.x * b.y - a.y * b.x
        };
    }

    [[gnu::const]]
    inline float length(Vec3 v) noexcept { 
        return std::sqrt(dot(v, v)); 
    }

    [[gnu::const]]
    inline Vec3 normalize(Vec3 v) noexcept {
        float len = length(v);

        if (len > 0.0001f) [[likely]] {
            return v * (1.0f / len);
        } else {
            return Vec3{ 0, 0, 0 };
        }
    }

    // Quaternions

    [[gnu::const]]
    inline Quat quat_from_euler(float pitch, float yaw, float roll) noexcept {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        return {
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        };
    }

    [[gnu::const]]
    inline Quat normalize(Quat q) noexcept {
        float len = std::sqrt(
            q.x * q.x + 
            q.y * q.y + 
            q.z * q.z + 
            q.w * q.w
        );

        if (len < 0.0001f) [[unlikely]] {
            return Quat::identity();
        }
        
        float inv = 1.0f / len;

        return {
            q.x * inv, 
            q.y * inv, 
            q.z * inv, 
            q.w * inv
        };
    }
    
    [[gnu::always_inline]] [[gnu::const]]
    inline constexpr Quat operator*(Quat a, Quat b) noexcept {
        return {
            a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
            a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
            a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
            a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
        };
    }

    [[gnu::const]]
    inline constexpr Vec3 rotate(Quat q, Vec3 v) noexcept {
        Vec3 u = { q.x, q.y, q.z };
        float s = q.w;
        Vec3 t = cross(u, v) * 2.0f;
        
        return v + (t * s) + cross(u, t);
    }

    // Matrix Functions

    // -ffast-math, -ftree-vectorize, & -funroll-loops help. Mat4 mul. is heavy
    [[gnu::pure]]
    inline constexpr Mat4 operator*(const Mat4& a, const Mat4& b) noexcept {
        Mat4 r = { 0 };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    r.m[i * 4 + j] += a.m[i * 4 + k] * b.m[k * 4 + j];
                }
            }
        }

        return r;
    }

    [[gnu::const]]
    inline constexpr Mat4 translate(Vec3 t) noexcept {
        Mat4 m = Mat4::identity();

        m.m[12] = t.x; 
        m.m[13] = t.y; 
        m.m[14] = t.z;

        return m;
    }

    [[gnu::const]]
    inline constexpr Mat4 scale(Vec3 s) noexcept {
        Mat4 m = Mat4::identity();

        m.m[0] = s.x; 
        m.m[5] = s.y;
        m.m[10] = s.z;

        return m;
    }

    [[gnu::const]]
    inline constexpr Mat4 mat4_rotate(Quat q) noexcept {
        Mat4 m = Mat4::identity();

        float xx = q.x * q.x;
        float yy = q.y * q.y;
        float zz = q.z * q.z;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;
        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;

        m.m[0] = 1 - 2 * (yy + zz); 
        m.m[1] = 2 * (xy + wz);     
        m.m[2] = 2 * (xz - wy);

        m.m[4] = 2 * (xy - wz);     
        m.m[5] = 1 - 2 * (xx + zz); 
        m.m[6] = 2 * (yz + wx);

        m.m[8] = 2 * (xz + wy);     
        m.m[9] = 2 * (yz - wx);     
        m.m[10] = 1 - 2 * (xx + yy);

        return m;
    }

    [[gnu::const]]
    inline Mat4 perspective(float fov, float aspect, float near_plane, float far_plane) noexcept {
        Mat4 m = { 0 };
        float f = 1.0f / std::tan(fov * 0.5f);

        m.m[0] = f / aspect;
        m.m[5] = f;
        m.m[10] = (far_plane + near_plane) / (near_plane - far_plane);
        m.m[11] = (2.0f * far_plane * near_plane) / (near_plane - far_plane);
        m.m[14] = -1.0f;

        return m;
    }

    [[gnu::const]]
    inline Mat4 look_at(Vec3 eye, Vec3 target, Vec3 up) noexcept {
        Vec3 f = normalize(target - eye);
        Vec3 r = normalize(cross(f, up));
        Vec3 u = cross(r, f);
        Mat4 m = Mat4::identity();

        m.m[0] = r.x;  
        m.m[1] = r.y;  
        m.m[2] = r.z;   
        m.m[3] = -dot(r, eye);

        m.m[4] = u.x;  
        m.m[5] = u.y;  
        m.m[6] = u.z;   
        m.m[7] = -dot(u, eye);

        m.m[8] = -f.x; 
        m.m[9] = -f.y; 
        m.m[10] = -f.z; 
        m.m[11] = dot(f, eye);

        return m;
    }

    // Frustum/Collision

    [[gnu::pure]]
    inline Frustum extract_frustum(const Mat4& vp) noexcept {
        Frustum f;
        const float* m = vp.m;
        
        auto set = [&](int i, float a, float b, float c, float d) {
            float len = std::sqrt(a * a + b * b + c * c);
            if (len > 0.0001f) [[likely]] { 
                a /= len; 
                b /= len; 
                c /= len; 
                d /= len; 
            };

            f.planes[i] = {
                a, b, c, d
            };
        };

        set(0, m[3] + m[0], m[7] + m[4], m[11] + m[8],  m[15] + m[12]);
        set(1, m[3] - m[0], m[7] - m[4], m[11] - m[8],  m[15] - m[12]);
        set(2, m[3] + m[1], m[7] + m[5], m[11] + m[9],  m[15] + m[13]);
        set(3, m[3] - m[1], m[7] - m[5], m[11] - m[9],  m[15] - m[13]);
        set(4, m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
        set(5, m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);

        return f;
    }

    [[gnu::hot]] [[gnu::pure]]
    inline constexpr bool cull_aabb(const Frustum& f, const Aabb& box) noexcept {
        for (int i = 0; i < 6; i++) {
            Vec4 p = f.planes[i];

            Vec3 positive = {
                p.x > 0 ? box.max.x : box.min.x,
                p.y > 0 ? box.max.y : box.min.y,
                p.z > 0 ? box.max.z : box.min.z
            };

            if (p.x * positive.x + p.y * positive.y + p.z * positive.z + p.w < 0) {
                return false;
            }
        }

        return true;
    }

    [[gnu::pure]]
    inline constexpr bool intersects(const Aabb& a, const Aabb& b) noexcept {
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) && (a.min.y <= b.max.y && a.max.y >= b.min.y) && (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    // Files
    
    [[gnu::cold]]
    inline std::vector<char> read_file(const char* path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file) [[unlikely]] {
            return {};
        }

        std::streamsize size = file.tellg();
        
        if (size <= 0) [[unlikely]] {
            return {};
        }

        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (file.read(buffer.data(), size)) {
            buffer.push_back(0);
            return buffer;
        }

        return {};
    }
}