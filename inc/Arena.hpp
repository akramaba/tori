#pragma once
#include <cstddef>
#include <cstdint>

namespace tori {
    class Arena {
    public:
        Arena() = default;
        
        [[gnu::cold]]
        ~Arena();

        // No weird copying or moving
        Arena(const Arena&) = delete;
        Arena& operator=(const Arena&) = delete;

        [[gnu::cold]]
        void init(size_t size_bytes);
        
        template <typename T>
        [[nodiscard]] T* alloc() noexcept {
            return static_cast<T*>(alloc_align(sizeof(T), alignof(T)));
        }

        template <typename T>
        [[nodiscard]] T* alloc_array(size_t count) noexcept {
            return static_cast<T*>(alloc_align(sizeof(T) * count, alignof(T)));
        }

        [[nodiscard]] void* alloc_bytes(size_t size, size_t align) noexcept {
            return alloc_align(size, align);
        }

        void reset() noexcept;

        [[nodiscard, gnu::pure]] 
        size_t marker() const noexcept;

        void reset_to(size_t marker) noexcept;

        [[nodiscard]] size_t used() const noexcept { 
            return offset_; 
        }

        [[nodiscard]] size_t total() const noexcept { 
            return total_size_;
        }
    private:
        [[nodiscard, gnu::hot]]
        void* alloc_align(size_t size, size_t align) noexcept;

        unsigned char* memory_ = nullptr;
        size_t total_size_ = 0;
        size_t offset_ = 0;
    };
}