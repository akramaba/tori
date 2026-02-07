// TODO: Also some more math and GLB unit tests to review/learn/do/etc. (+ more comments)

#include "../inc/Model.hpp"
#include "../lib/miniglad.h"

#define CGLTF_IMPLEMENTATION
#define CGLTF_WRITE_DISABLE
#include "../lib/cgltf.h"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <algorithm>

namespace tori {
    Shader Model::skin_shader_;

    static Mat4 identity_mat() {
        return {{ 
            1, 0, 0, 0, 
            0, 1, 0, 0, 
            0, 0, 1, 0, 
            0, 0, 0, 1 
        }};
    }

    static Quat identity_quat() {
        return { 0, 0, 0, 1 };
    }

    static Mat4 mul(const Mat4& a, const Mat4& b) {
        Mat4 r = { 0 };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                float sum = 0.0f;

                for (int k = 0; k < 4; k++) {
                    sum += a.m[i * 4 + k] * b.m[k * 4 + j];
                }
                
                r.m[i * 4 + j] = sum;
            }
        }

        return r;
    }

    static Mat4 invert(Mat4 m) {
        const float* a = m.m;
        Mat4 inv;
        float* o = inv.m;
        
        // Unrolled Laplace
        float s0 = a[0] * a[5] - a[4] * a[1];
        float s1 = a[0] * a[6] - a[4] * a[2];
        float s2 = a[0] * a[7] - a[4] * a[3];
        float s3 = a[1] * a[6] - a[5] * a[2];
        float s4 = a[1] * a[7] - a[5] * a[3];
        float s5 = a[2] * a[7] - a[6] * a[3];
        float c5 = a[10] * a[15] - a[14] * a[11];
        float c4 = a[9] * a[15] - a[13] * a[11];
        float c3 = a[9] * a[14] - a[13] * a[10];
        float c2 = a[8] * a[15] - a[12] * a[11];
        float c1 = a[8] * a[14] - a[12] * a[10];
        float c0 = a[8] * a[13] - a[12] * a[9];
        
        float det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
        if (std::fabs(det) < 1e-10f) {
            return identity_mat(); // Can't invert
        }

        float invdet = 1.0f / det;
        
        // Adjugate matrix
        o[0] = (a[5] * c5 - a[6] * c4 + a[7] * c3) * invdet;
        o[1] = (-a[1] * c5 + a[2] * c4 - a[3] * c3) * invdet;
        o[2] = (a[13] * s5 - a[14] * s4 + a[15] * s3) * invdet;
        o[3] = (-a[9] * s5 + a[10] * s4 - a[11] * s3) * invdet;
        o[4] = (-a[4] * c5 + a[6] * c2 - a[7] * c1) * invdet;
        o[5] = (a[0] * c5 - a[2] * c2 + a[3] * c1) * invdet;
        o[6] = (-a[12] * s5 + a[14] * s2 - a[15] * s1) * invdet;
        o[7] = (a[8] * s5 - a[10] * s2 + a[11] * s1) * invdet;
        o[8] = (a[4] * c4 - a[5] * c2 + a[7] * c0) * invdet;
        o[9] = (-a[0] * c4 + a[1] * c2 - a[3] * c0) * invdet;
        o[10] = (a[12] * s4 - a[13] * s2 + a[15] * s0) * invdet;
        o[11] = (-a[8] * s4 + a[9] * s2 - a[11] * s0) * invdet;
        o[12] = (-a[4] * c3 + a[5] * c1 - a[6] * c0) * invdet;
        o[13] = (a[0] * c3 - a[1] * c1 + a[2] * c0) * invdet;
        o[14] = (-a[12] * s3 + a[13] * s1 - a[14] * s0) * invdet;
        o[15] = (a[8] * s3 - a[9] * s1 + a[10] * s0) * invdet;
        
        return inv;
    }

    static Quat slerp(Quat a, Quat b, float t) {
        float d = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

        if (d < 0) { 
            b.x = -b.x; 
            b.y = -b.y; 
            b.z = -b.z; 
            b.w = -b.w; 
            d = -d; 
        }
        
        if (d > 0.9995f) {
            Quat r = {
                a.x + (b.x - a.x) * t, 
                a.y + (b.y - a.y) * t, 
                a.z + (b.z - a.z) * t, 
                a.w + (b.w - a.w) * t
            };

            return normalize(r);
        }

        float theta = std::acos(d);
        float sn = std::sin(theta);
        float wa = std::sin((1.0f - t) * theta) / sn;
        float wb = std::sin(t * theta) / sn;

        return {
            a.x * wa + b.x * wb, 
            a.y * wa + b.y * wb, 
            a.z * wa + b.z * wb, 
            a.w * wa + b.w * wb
        };
    }

    static Vec3 lerp_vec3(Vec3 a, Vec3 b, float t) {
        return {
            a.x + (b.x - a.x) * t, 
            a.y + (b.y - a.y) * t, 
            a.z + (b.z - a.z) * t
        };
    }

    static Mat4 compose_srt(Vec3 s, Quat r, Vec3 t) {
        Mat4 ms = scale(s);
        Mat4 mr = mat4_rotate(r);
        Mat4 mt = translate(t);

        return mul(ms, mul(mr, mt)); 
    }

    static int find_node_idx(cgltf_data* data, cgltf_node* node) {
        if (!node) return -1;
        return (int)(node - data->nodes);
    }

    // TODO: Only one shader, but consider not embedding it here if things grow
    static const char* skin_vert = R"(
        #version 330 core

        layout(location = 0) in vec3 a_pos;
        layout(location = 1) in vec3 a_norm;
        layout(location = 2) in vec2 a_uv;
        layout(location = 3) in vec4 a_joints;
        layout(location = 4) in vec4 a_weights;

        uniform mat4 u_mvp;
        uniform mat4 u_model;
        uniform mat4 u_joints[128];
        uniform int u_skinned;

        out vec3 v_norm;
        out vec2 v_uv;

        void main() 
        {
            mat4 skin = mat4(1.0);

            if (u_skinned == 1) 
            {
                skin = u_joints[int(a_joints.x)] * a_weights.x +
                    u_joints[int(a_joints.y)] * a_weights.y +
                    u_joints[int(a_joints.z)] * a_weights.z +
                    u_joints[int(a_joints.w)] * a_weights.w;
            }

            vec4 pos = skin * vec4(a_pos, 1.0);

            v_norm = mat3(u_model) * mat3(skin) * a_norm;
            v_uv = a_uv;

            gl_Position = u_mvp * pos;
        }
    )";

    // TODO: Support alpha and textures
    static const char* skin_frag = R"(
        #version 330 core
        
        in vec3 v_norm;
        in vec2 v_uv;

        uniform vec3 u_color;
        uniform vec3 u_sun_dir;

        out vec4 frag;

        void main() 
        {
            vec3 n = normalize(v_norm);

            float d = max(dot(n, u_sun_dir), 0.0) * 0.8 + 0.2;

            frag = vec4(
                u_color * d, 
                1.0
            );
        }
    )";

    void Model::ensure_shader() {
        if (!skin_shader_.is_valid()) {
            skin_shader_.load(skin_vert, skin_frag);
        }
    }

    Model::~Model() {
        for (auto& m : meshes_) {
            glDeleteVertexArrays(1, &m.vao);
            glDeleteBuffers(1, &m.vbo);
            glDeleteBuffers(1, &m.ebo);
        }
    }

    Model* Model::load(const char* path) {
        ensure_shader();

        cgltf_options opt = {};
        cgltf_data* data = nullptr;
        
        if (cgltf_parse_file(&opt, path, &data) != cgltf_result_success) {
            return nullptr;
        }

        if (cgltf_load_buffers(&opt, data, path) != cgltf_result_success) {
            cgltf_free(data);
            return nullptr;
        }

        Model* m = new Model();
        
        // Setup nodes
        m->nodes_.resize(data->nodes_count);
        for (size_t i = 0; i < data->nodes_count; ++i) {
            cgltf_node* n = &data->nodes[i];
            Node& node = m->nodes_[i];

            node.parent = n->parent ? find_node_idx(data, n->parent) : -1;
            
            // TODO: Shorten each struct init
            node.translation = n->has_translation ? Vec3{ n->translation[0], n->translation[1], n->translation[2] } : Vec3{ 0, 0, 0 };
            node.rotation = n->has_rotation ? Quat{ n->rotation[0], n->rotation[1], n->rotation[2], n->rotation[3] } : identity_quat();
            node.scale = n->has_scale ? Vec3{ n->scale[0], n->scale[1], n->scale[2] } : Vec3{ 1, 1, 1 };

            node.local = compose_srt(node.scale, node.rotation, node.translation);
        }

        // Skins
        if (data->skins_count > 0) {
            cgltf_skin* skin = &data->skins[0];
            int joint_count = (int)skin->joints_count;

            if (joint_count > 128) {
                joint_count = 128; // Max
            }

            m->joint_nodes_.resize(joint_count);
            m->inverse_binds_.resize(joint_count);
            m->joint_matrices_.resize(joint_count, identity_mat());

            for (int i = 0; i < joint_count; ++i) {
                m->joint_nodes_[i] = find_node_idx(data, skin->joints[i]);

                if (skin->inverse_bind_matrices) {
                    cgltf_accessor_read_float(skin->inverse_bind_matrices, i, m->inverse_binds_[i].m, 16);
                } else {
                    m->inverse_binds_[i] = identity_mat();
                }
            }
        }

        // Meshes
        for (size_t i = 0; i < data->meshes_count; ++i) {
            cgltf_mesh* gm = &data->meshes[i];

            for (size_t pi = 0; pi < gm->primitives_count; ++pi) {
                cgltf_primitive* prim = &gm->primitives[pi];

                Mesh mesh;
                mesh.base_color = {
                    1.0f, 
                    1.0f, 
                    1.0f, 
                    1.0f
                };

                if (prim->material && prim->material->has_pbr_metallic_roughness) {
                    float* c = prim->material->pbr_metallic_roughness.base_color_factor;
                    
                    mesh.base_color = { 
                        c[0], 
                        c[1], 
                        c[2], 
                        c[3] 
                    };
                }

                cgltf_accessor *pos = 0;
                cgltf_accessor *norm = 0;
                cgltf_accessor *uv = 0;
                cgltf_accessor *jnt = 0;
                cgltf_accessor *wgt = 0;

                for (size_t ai = 0; ai < prim->attributes_count; ++ai) {
                    auto& attr = prim->attributes[ai];

                    if (attr.type == cgltf_attribute_type_position) {
                        pos = attr.data;
                    } else if (attr.type == cgltf_attribute_type_normal) {
                        norm = attr.data;
                    } else if (attr.type == cgltf_attribute_type_texcoord) {
                        uv = attr.data;
                    } else if (attr.type == cgltf_attribute_type_joints) {
                        jnt = attr.data;
                    } else if (attr.type == cgltf_attribute_type_weights) {
                        wgt = attr.data;
                    }
                }

                if (!pos) continue;

                size_t vert_count = pos->count;
                
                std::vector<float> vertices;
                vertices.reserve(vert_count * 16); // pos(3) + norm(3) + uv(2) + joints(4) + weights(4)

                mesh.bounds.min = { 1e30f, 1e30f, 1e30f };
                mesh.bounds.max = { -1e30f, -1e30f, -1e30f };

                for (size_t vi = 0; vi < vert_count; ++vi) {
                    float v[16]; // local buffer
                    cgltf_accessor_read_float(pos, vi, v, 3);
                    
                    // Min bounds
                    mesh.bounds.min.x = std::min(mesh.bounds.min.x, v[0]);
                    mesh.bounds.min.y = std::min(mesh.bounds.min.y, v[1]);
                    mesh.bounds.min.z = std::min(mesh.bounds.min.z, v[2]);

                    // Max bounds
                    mesh.bounds.max.x = std::max(mesh.bounds.max.x, v[0]);
                    mesh.bounds.max.y = std::max(mesh.bounds.max.y, v[1]);
                    mesh.bounds.max.z = std::max(mesh.bounds.max.z, v[2]);

                    if (norm) {
                        cgltf_accessor_read_float(norm, vi, v + 3, 3);
                    } else { 
                        v[3] = 0; 
                        v[4] = 1; 
                        v[5] = 0; 
                    }

                    if (uv) {
                        cgltf_accessor_read_float(uv, vi, v + 6, 2);
                    } else { 
                        v[6] = 0; 
                        v[7] = 0; 
                    }

                    if (jnt) {
                        float j[4]; 
                        cgltf_accessor_read_float(jnt, vi, j, 4);

                        v[8] = j[0]; 
                        v[9] = j[1]; 
                        v[10] = j[2]; 
                        v[11] = j[3];
                    } else { 
                        v[8] = 0; 
                        v[9] = 0; 
                        v[10] = 0; 
                        v[11] = 0; 
                    }

                    if (wgt) {
                        cgltf_accessor_read_float(wgt, vi, v + 12, 4);
                    } else if (jnt) { 
                        v[12] = 1; 
                        v[13] = 0; 
                        v[14] = 0; 
                        v[15] = 0; 
                    } else { 
                        // Default if neither exist
                        v[12] = 1; 
                        v[13] = 0; 
                        v[14] = 0; 
                        v[15] = 0; 
                    }

                    vertices.insert(vertices.end(), v, v + 16);
                }

                // Min global bounds
                m->bounds_.min.x = std::min(m->bounds_.min.x, mesh.bounds.min.x);
                m->bounds_.min.y = std::min(m->bounds_.min.y, mesh.bounds.min.y);
                m->bounds_.min.z = std::min(m->bounds_.min.z, mesh.bounds.min.z);

                // Max global bounds
                m->bounds_.max.x = std::max(m->bounds_.max.x, mesh.bounds.max.x);
                m->bounds_.max.y = std::max(m->bounds_.max.y, mesh.bounds.max.y);
                m->bounds_.max.z = std::max(m->bounds_.max.z, mesh.bounds.max.z);

                std::vector<unsigned int> indices;

                if (prim->indices) {
                    size_t cnt = prim->indices->count;
                    indices.resize(cnt);

                    for (size_t k = 0; k < cnt; ++k) {
                        indices[k] = (unsigned int)cgltf_accessor_read_index(prim->indices, k);
                    }
                } else {
                    indices.resize(vert_count);

                    for (size_t k = 0; k < vert_count; ++k) {
                        indices[k] = k;
                    }
                }

                mesh.index_count = (int)indices.size();

                // GL buffer setup

                glGenVertexArrays(1, &mesh.vao);
                glGenBuffers(1, &mesh.vbo);
                glGenBuffers(1, &mesh.ebo);

                glBindVertexArray(mesh.vao);

                glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
                glBufferData(
                    GL_ARRAY_BUFFER, 
                    vertices.size() * sizeof(float), 
                    vertices.data(), 
                    GL_STATIC_DRAW
                );

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
                glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER, 
                    indices.size() * sizeof(unsigned int), 
                    indices.data(), 
                    GL_STATIC_DRAW
                );

                int stride = 16 * sizeof(float);
                int off = 0;

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(off * sizeof(float))); 
                glEnableVertexAttribArray(0); 
                off += 3;

                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(off * sizeof(float))); 
                glEnableVertexAttribArray(1); 
                off += 3;

                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(off * sizeof(float))); 
                glEnableVertexAttribArray(2); 
                off += 2;

                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * sizeof(float))); 
                glEnableVertexAttribArray(3); 
                off += 4;

                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * sizeof(float))); 
                glEnableVertexAttribArray(4);

                glBindVertexArray(0);

                m->meshes_.push_back(mesh);
            }
        }

        // Animations
        m->animations_.resize(data->animations_count);
        for (size_t i = 0; i < data->animations_count; ++i) {
            cgltf_animation* ga = &data->animations[i];
            Anim& anim = m->animations_[i];
            
            for (size_t c = 0; c < ga->channels_count; ++c) {
                cgltf_animation_channel* gc = &ga->channels[c];
                Channel ch;
                ch.node_index = find_node_idx(data, gc->target_node);

                if (ch.node_index < 0) {
                    continue; // Invalid
                }

                if (gc->target_path == cgltf_animation_path_type_translation) {
                    ch.path = 0;
                }

                ch.path = (int)gc->target_path; 

                cgltf_animation_sampler* gs = gc->sampler;
                ch.interpolation = (int)gs->interpolation;
                
                ch.times.resize(gs->input->count);
                cgltf_accessor_unpack_floats(gs->input, ch.times.data(), ch.times.size());

                if (!ch.times.empty() && ch.times.back() > anim.duration) {
                    anim.duration = ch.times.back();
                }

                size_t num_comps = cgltf_num_components(gs->output->type);
                ch.values.resize(gs->output->count * num_comps);
                cgltf_accessor_unpack_floats(gs->output, ch.values.data(), ch.values.size());

                anim.channels.push_back(ch);
            }
        }

        cgltf_free(data);

        return m;
    }

    void Model::animate(int anim_idx, float time) {
        if (anim_idx < 0 || anim_idx >= (int)animations_.size()) {
            return;
        }

        Anim& anim = animations_[anim_idx];

        if (anim.duration > 0) {
            time = std::fmod(time, anim.duration);
        }

        for (const auto& ch : anim.channels) {
            if (ch.node_index < 0 || ch.times.empty()) continue;

            size_t k0 = 0;
            size_t k1 = 0;

            for (size_t k = 0; k < ch.times.size() - 1; ++k) {
                if (time >= ch.times[k] && time < ch.times[k + 1]) { 
                    k0 = k;
                    k1 = k + 1;

                    break; 
                }

                if (k == ch.times.size() - 2) { 
                    k0 = k1 = ch.times.size() - 1; 
                }
            }

            float t = 0;

            if (k0 != k1 && ch.times[k1] != ch.times[k0]) {
                t = (time - ch.times[k0]) / (ch.times[k1] - ch.times[k0]);
            }

            // 1 = trans, 2 = rot, 3 = scale
            // Stride - Rot(2) is vec4, others vec3
            int stride = (ch.path == 2) ? 4 : 3;

            const float* v0 = &ch.values[k0 * stride];
            const float* v1 = &ch.values[k1 * stride];

            Node& node = nodes_[ch.node_index];

            if (ch.path == 1) { // Translation
                Vec3 a = {
                    v0[0],
                    v0[1],
                    v0[2]
                };

                Vec3 b = {
                    v1[0],
                    v1[1],
                    v1[2]
                };

                if (ch.interpolation == 1) {
                    node.translation = a;
                } else {
                    node.translation = lerp_vec3(a, b, t);
                }
            } else if (ch.path == 2) { // Rotation
                Quat a = {
                    v0[0],
                    v0[1],
                    v0[2],
                    v0[3]
                };

                Quat b = {
                    v1[0],
                    v1[1],
                    v1[2],
                    v1[3]
                };

                if (ch.interpolation == 1) {
                    node.rotation = a;
                } else {
                    node.rotation = slerp(a, b, t);
                }
            } else if (ch.path == 3) { // Scale
                Vec3 a = {
                    v0[0],
                    v0[1],
                    v0[2]
                };

                Vec3 b = {
                    v1[0],
                    v1[1],
                    v1[2]
                };

                if (ch.interpolation == 1) {
                    node.scale = a;
                } else {
                    node.scale = lerp_vec3(a, b, t);
                }
            }
        }

        // Update transforms (SRT)
        for (auto& node : nodes_) {
            node.local = compose_srt(node.scale, node.rotation, node.translation);
        }

        // Local * Parent
        for (size_t i = 0; i < nodes_.size(); ++i) {
            if (nodes_[i].parent >= 0) {
                nodes_[i].global = mul(nodes_[i].local, nodes_[nodes_[i].parent].global);
            } else {
                nodes_[i].global = nodes_[i].local;
            }
        }

        // Inverse, global[node]
        for (size_t i = 0; i < joint_nodes_.size(); ++i) {
            int node_idx = joint_nodes_[i];
            if (node_idx >= 0) {
                joint_matrices_[i] = mul(inverse_binds_[i], nodes_[node_idx].global);
            }
        }
    }

    void Model::draw(const Mat4& mvp) {
        if (!skin_shader_.is_valid()) {
            return;
        }

        skin_shader_.use();
        
        // ? Try caching instead of sending a bool every draw (for speed)
        bool skinned = !joint_nodes_.empty();
        glUniform1i(glGetUniformLocation(skin_shader_.id(), "u_skinned"), skinned ? 1 : 0);
        
        if (skinned) {
            glUniformMatrix4fv(
                glGetUniformLocation(skin_shader_.id(), "u_joints"), 
                (int)joint_matrices_.size(), 
                GL_FALSE, 
                (float*)joint_matrices_.data()
            );
        }

        skin_shader_.set("u_mvp", mvp);
        skin_shader_.set("u_model", identity_mat());
        skin_shader_.set("u_sun_dir", Vec3{ 0.5f, 1.0f, 0.3f });

        for (const auto& m : meshes_) {
            skin_shader_.set("u_color", Vec3{ m.base_color.x, m.base_color.y, m.base_color.z });

            glBindVertexArray(m.vao);
            glDrawElements(GL_TRIANGLES, m.index_count, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
    }

    Aabb Model::bounds() const { 
        return bounds_;
    }

    int Model::anim_count() const { 
        return (int)animations_.size(); 
    }

    float Model::anim_duration(int idx) const { 
        if (idx >= 0 && idx < (int)animations_.size()) {
            return animations_[idx].duration;
        }

        return 0.0f;
    }
}