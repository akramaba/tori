## 2/8/26 - Optimize Math.hpp & Model.cpp with compiler attributes, memory alignment, constexpr, and noexcept

### Compiler Attributes

I added a few GNU and standard C++ attributes to optimize speed and function overhead.

- `[[gnu::const]]` - Added to math functions that read only their arguments by value (dot, cross, and all other math helpers). The compiler needs to know that a function does not read global memory or has side effects (like a counter) in order to pre-calculate results.
- `[[gnu::pure]]` - Added to functions that take pointers or references (like matrix multiplication and getters). This ensures that while the function reads memory, it does not modify global state.
- `[[gnu::hot]]` - Added to the animate and draw functions for models. They are called every frame, so this should make the linker group those functions together in the executable to improve instruction cache performance.
- `[[gnu::cold]]` - Added to the load function for models. This moves the large and rarely used loading logic to a different part of the binary (good for the cache).
- `[[likely]]` & `[[unlikely]]` - Added to error checks and animation interpolation branches. This prioritizes the common path, which speeds up the pipeline.

### Memory alignment

I did memory alignment for math structures to ensure they work with SIMD instructions and to optimize the cache.

- Added `[[gnu::aligned(16)]]` to Vec4, Quat, and Mat4. This is important for SSE/AVX performance because it starts structures on a 16-byte boundary.
- The Mat4 structure is 64 bytes (16 floats), which is the same size as a CPU cache line. I added the 16-byte alignment because it now wouldn't potentially use two cache lines.
- Vec3 has an extra float as padding for faster block processing.

### Constexpr usage

I added compile-time evaluation for math functions whenever possible. Simple arithmetic operators (Vec2/Vec3 addition, multiplication), constants, and matrix builders (translate, scale) are now constexpr. If code calls these with constant values, the compiler computes the result during the build process instead of at runtime.

### Noexcept usage

I added noexcept to all math operators and helpers/getters. Since these functions perform just arithmetic and cannot throw exceptions, this keyword tells the compiler to omit stack-unwind tables. This reduces the binary size and instruction overhead for every function call.

### Miscellaneous

For `Model.cpp`:

- Changed glGetUniformLocation calls for skinned and joints uniforms to use static local variables (faster). Before, this happened every frame.
- Removed internal functions already provided by `Math.hpp`.
