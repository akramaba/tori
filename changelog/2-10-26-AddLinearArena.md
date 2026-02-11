## 2/10/26 - Add stack-based linear arena allocator

### Arena Allocator

I added a custom Linear Arena (Stack Allocator) for memory allocations. Its purpose is to replace general-purpose functions, such as `malloc` and `new`, in the hot path with a deterministic and faster method.

- Memory is allocated by simply incrementing an offset integer, which allows for O(1) allocation. This avoids the overhead of searching for free blocks and ensures deterministic behavior.
- Added `marker()` and `reset_to()` functions. This lets me rewind the allocator to a previous state, which can free a batch of objects without overwriting them.
- Used specific compiler attributes for optimization, including `[[nodiscard]]` to prevent logic bugs where memory is requested but never referenced.
- The allocator performs bitwise alignment `(ptr + (align - 1)) & ~(align - 1)` to ensure that any type requested (specifically `Mat4`/`Vec4`) respects its natural boundary (16/64 bytes). This is also for SIMD requirements, as without it, there could be cache-line splits for large math structures.