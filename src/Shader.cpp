#include "../inc/Shader.hpp"
#include "../lib/miniglad.h"
#include <iostream>

namespace tori {
    Shader::~Shader() {
        free();
    }

    Shader::Shader(Shader&& other) : id_(other.id_) {
        other.id_ = 0;
    }

    Shader& Shader::operator=(Shader&& other) {
        if (this != &other) {
            free();

            id_ = other.id_;
            other.id_ = 0;
        }

        return *this;
    }

    static unsigned int compile(unsigned int type, const char* src) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glDeleteShader(shader);
            return 0;
        }
        
        return shader;
    }

    bool Shader::load(const char* vert_src, const char* frag_src) {
        unsigned int vert = compile(GL_VERTEX_SHADER, vert_src);
        unsigned int frag = compile(GL_FRAGMENT_SHADER, frag_src);

        if (!vert || !frag) {
            if (vert) {
                glDeleteShader(vert);
            }
            
            if (frag) {
                glDeleteShader(frag);
            }

            return false;
        }

        unsigned int program = glCreateProgram();
        glAttachShader(program, vert);
        glAttachShader(program, frag);
        glLinkProgram(program);

        // Clean up
        glDeleteShader(vert);
        glDeleteShader(frag);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            glDeleteProgram(program);
            return false;
        }

        // If old program, free it first
        free();
        id_ = program;

        return true;
    }

    void Shader::use() const {
        if (id_) {
            glUseProgram(id_);
        }
    }

    void Shader::free() {
        if (id_) {
            glDeleteProgram(id_);
            id_ = 0;
        }
    }

    // Uniforms

    void Shader::set(const char* name, int value) const {
        if (id_) {
            glUniform1i(glGetUniformLocation(id_, name), value);
        }
    }

    void Shader::set(const char* name, float value) const {
        if (id_) {
            glUniform1f(glGetUniformLocation(id_, name), value);
        }
    }

    void Shader::set(const char* name, Vec2 value) const {
        if (id_) {
            glUniform2f(glGetUniformLocation(id_, name), value.x, value.y);
        }
    }

    void Shader::set(const char* name, Vec3 value) const {
        if (id_) {
            glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
        }
    }

    void Shader::set(const char* name, Vec4 value) const {
        if (id_) {
            glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z, value.w);
        }
    }

    void Shader::set(const char* name, const Mat4& value) const {
        if (id_) {
            glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_TRUE, value.m);
        }
    }
}