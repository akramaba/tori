#include "../inc/Octree.hpp"

namespace tori {
    Octree::Octree(const Aabb& root_bounds) {
        root_ = std::make_unique<Node>();
        root_->bounds = root_bounds;
    }

    Octree::~Octree() = default;

    [[gnu::cold]]
    void Octree::clear() {
        Aabb old_bounds = root_->bounds;
        root_ = std::make_unique<Node>();
        root_->bounds = old_bounds;
    }

    void Octree::insert(void* data, const Aabb& bounds) {
        if (!intersects(root_->bounds, bounds)) [[unlikely]] {
            return; // Ignore
        }

        insert_impl(root_.get(), {
            data,
            bounds
        }, 0);
    }

    [[gnu::hot]]
    void Octree::insert_impl(Node* node, const OctreeElement& elem, size_t depth) {
        bool is_leaf = node->is_leaf();
        
        if (!is_leaf) {
            int index = get_octant(node->bounds, elem.bounds);
            if (index != -1) {
                insert_impl(node->children[index].get(), elem, depth + 1);
                return;
            }
        }

        node->elements.push_back(elem);

        if (node->elements.size() > NODE_CAPACITY && depth < MAX_DEPTH) {
            if (is_leaf) {
                split(node);
                
                // Re-distribute existing elements

                std::vector<OctreeElement> old_elements = std::move(node->elements);

                node->elements.clear();
                
                for (const auto& e : old_elements) {
                    insert_impl(node, e, depth);
                }
            }
        }
    }

    void Octree::split(Node* node) {
        Vec3 min = node->bounds.min;
        Vec3 max = node->bounds.max;
        Vec3 center = (min + max) * 0.5f;

        // Unrolled, subdivide bounds
        
        node->children[0] = std::make_unique<Node>();
        node->children[0]->bounds = {
            min,
            0.f,
            center,
            0.f
        };

        node->children[1] = std::make_unique<Node>();
        node->children[1]->bounds = { 
            { center.x, min.y, min.z },
            0.f,
            { max.x, center.y, center.z },
            0.f
        };

        node->children[2] = std::make_unique<Node>();
        node->children[2]->bounds = { 
            { min.x, min.y, center.z },
            0.f,
            { center.x, center.y, max.z },
            0.f 
        };

        node->children[3] = std::make_unique<Node>();
        node->children[3]->bounds = { 
            { center.x, min.y, center.z },
            0.f,
            { max.x, center.y, max.z },
            0.f 
        };

        node->children[4] = std::make_unique<Node>();
        node->children[4]->bounds = { 
            { min.x, center.y, min.z },
            0.f,
            { center.x, max.y, center.z },
            0.f 
        };

        node->children[5] = std::make_unique<Node>();
        node->children[5]->bounds = { 
            { center.x, center.y, min.z },
            0.f,
            { max.x, max.y, center.z },
            0.f 
        };

        node->children[6] = std::make_unique<Node>();
        node->children[6]->bounds = { 
            { min.x, center.y, center.z },
            0.f,
            { center.x, max.y, max.z },
            0.f 
        };

        node->children[7] = std::make_unique<Node>();
        node->children[7]->bounds = { 
            center,
            0.f,
            max,
            0.f 
        };
    }

    // Returns index 0-7 if fits entirely, -1 if not
    // TODO: Optimize?
    [[nodiscard, gnu::pure]]
    int Octree::get_octant(const Aabb& node_bounds, const Aabb& obj_bounds) const noexcept {
        Vec3 center = (node_bounds.min + node_bounds.max) * 0.5f;

        bool right = obj_bounds.min.x >= center.x;
        bool left = obj_bounds.max.x <= center.x;
        bool top = obj_bounds.min.y >= center.y;
        bool bottom = obj_bounds.max.y <= center.y;
        bool back = obj_bounds.min.z >= center.z;
        bool front = obj_bounds.max.z <= center.z;

        if (left) {
            if (bottom) {
                if (front) {
                    return 0;
                }

                if (back) {
                    return 2;
                }
            } else if (top) {
                if (front) {
                    return 4;
                }

                if (back) {
                    return 6;
                }
            }
        } else if (right) {
            if (bottom) {
                if (front) {
                    return 1;
                }

                if (back) {
                    return 3;
                }
            } else if (top) {
                if (front) {
                    return 5;
                }

                if (back) {
                    return 7;
                }
            }
        }

        return -1; // Straddles a plane, keep in parent
    }

    [[nodiscard]] std::vector<void*> Octree::query(const Frustum& frustum) const {
        std::vector<void*> results;
        results.reserve(64); // Avoid reallocs for speed

        query_frustum_impl(root_.get(), frustum, results);

        return results;
    }

    [[nodiscard]] std::vector<void*> Octree::query(const Aabb& range) const {
        std::vector<void*> results;
        results.reserve(64);

        query_aabb_impl(root_.get(), range, results);
        
        return results;
    }

    [[gnu::hot]]
    void Octree::query_frustum_impl(const Node* node, const Frustum& frustum, std::vector<void*>& results) const {
        if (!cull_aabb(frustum, node->bounds)) {
            return;
        }

        for (const auto& elem : node->elements) {
            if (cull_aabb(frustum, elem.bounds)) {
                results.push_back(elem.data);
            }
        }

        if (!node->is_leaf()) {
            for (int i = 0; i < 8; ++i) {
                query_frustum_impl(node->children[i].get(), frustum, results);
            }
        }
    }

    void Octree::query_aabb_impl(const Node* node, const Aabb& range, std::vector<void*>& results) const {
        if (!intersects(range, node->bounds)) {
            return;
        }

        for (const auto& elem : node->elements) {
            if (intersects(range, elem.bounds)) {
                results.push_back(elem.data);
            }
        }

        if (!node->is_leaf()) {
            for (int i = 0; i < 8; ++i) {
                query_aabb_impl(node->children[i].get(), range, results);
            }
        }
    }
}