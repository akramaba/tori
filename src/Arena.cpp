#include "../inc/Arena.hpp"
#include <cstdlib>

namespace tori {
    [[gnu::cold]]
    Arena::~Arena() {
        std::free(memory_);
    }

    [[gnu::cold]]
    void Arena::init(size_t size_bytes) {
        std::free(memory_);

        memory_ = static_cast<unsigned char*>(std::malloc(size_bytes));
        total_size_ = size_bytes;
        offset_ = 0;
    }

    [[nodiscard, gnu::hot]]
    void* Arena::alloc_align(size_t size, size_t align) noexcept {
        uintptr_t curr_ptr = reinterpret_cast<uintptr_t>(memory_) + offset_;

        // Align forward to next multiple of align (needs to be power of 2)
        uintptr_t aligned_ptr = (curr_ptr + (align - 1)) & ~(align - 1);
        
        size_t total_needed = (aligned_ptr - curr_ptr) + size;

        if (offset_ + total_needed > total_size_) [[unlikely]] {
            return nullptr; // Full
        }

        offset_ += total_needed;

        return reinterpret_cast<void*>(aligned_ptr);
    }

    void Arena::reset() noexcept {
        offset_ = 0;
    }

    [[nodiscard, gnu::pure]]
    size_t Arena::marker() const noexcept {
        return offset_;
    }

    void Arena::reset_to(size_t marker) noexcept {
        if (marker <= offset_) {
            offset_ = marker;
        }
    }
}