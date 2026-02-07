#pragma once

#include "Math.hpp"
#include <string>

namespace tori {
    class Shader {
    public:
        Shader() = default;
        ~Shader();

        Shader(Shader&& other);
        Shader& operator=(Shader&& other);

        bool load(const char* vert_src, const char* frag_src);
        
        void use() const;
        void free();

        // Set uniforms
        void set(const char* name, int value) const;
        void set(const char* name, float value) const;
        void set(const char* name, Vec2 value) const;
        void set(const char* name, Vec3 value) const;
        void set(const char* name, Vec4 value) const;
        void set(const char* name, const Mat4& value) const;

        bool is_valid() const { 
            return id_ != 0; 
        }

        unsigned int id() const { 
            return id_; 
        }
    private:
        unsigned int id_ = 0; // GLuint
    };
}