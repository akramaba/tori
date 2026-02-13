#pragma once

#include "Math.hpp"
#include <vector>
#include <memory>

namespace tori {
    struct OctreeElement {
        void* data;
        Aabb bounds;
    };

    class Octree {
    public:
        static constexpr size_t MAX_DEPTH = 8;
        static constexpr size_t NODE_CAPACITY = 16;

        Octree(const Aabb& root_bounds);
        ~Octree();

        Octree(const Octree&) = delete;
        Octree& operator=(const Octree&) = delete;

        void insert(void* data, const Aabb& bounds);

        [[gnu::cold]]
        void clear();

        [[nodiscard]] std::vector<void*> query(const Frustum& frustum) const;

        [[nodiscard]] std::vector<void*> query(const Aabb& range) const;
    private:
        struct Node {
            Aabb bounds;
            std::vector<OctreeElement> elements;
            std::unique_ptr<Node> children[8]; // nullptr if leaf
            
            [[nodiscard]] bool is_leaf() const noexcept { 
                return children[0] == nullptr; 
            }
        };

        std::unique_ptr<Node> root_;

        void insert_impl(Node* node, const OctreeElement& elem, size_t depth);

        void split(Node* node);

        void query_frustum_impl(const Node* node, const Frustum& frustum, std::vector<void*>& results) const;

        void query_aabb_impl(const Node* node, const Aabb& range, std::vector<void*>& results) const;
        
        // Determines which octant fits the bounds entirely
        [[nodiscard, gnu::pure]]
        int get_octant(const Aabb& node_bounds, const Aabb& obj_bounds) const noexcept;
    };
}