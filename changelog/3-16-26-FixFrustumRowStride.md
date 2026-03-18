## 3/16/26 - Fix frustum culling to extract planes using row strides

### Frustum Culling

I fixed a bug in the frustum plane extraction where the culling would work correctly when zooming in/out, but geometry would clip out when translating the camera left, right, up, or down.

- The `extract_frustum(const Mat4& vp)` function was incorrectly built with column-striding (`m[3] + m[0]`, etc.).
- Because the projection-view matrix multiplication produces a structure that evaluates as transposed `(View * Proj)^T` relative to standard memory layout expectations, the previous logic was accidentally extracting the columns of the transposed matrix instead of its rows.
- Updated the logic to stride across the actual rows in memory (`m[12] + m[0]`, `m[13] + m[1]`, etc.). This calculates orthogonal planes pointing correctly outward from the camera.