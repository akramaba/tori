## 2/12/26 - Add Octree.cpp implementation with insertion and query logic

### Octree Implementation

I added a Region Octree to eventually handle spatial partitioning for scenes. Its goal is to optimize render and physics queries by organizing objects into a recursive hierarchical grid, which would replace the current linear iteration.

- Implemented recursive subdivision where the root bounding box splits into eight octants once a node exceeds `NODE_CAPACITY`.
- Handles AABBs: objects that fit entirely within a child quadrant are pushed down, while objects that straddle the center division planes go into the parent node. This ensures spatial containment.
- Reserved vector capacity (`reserve(64)`) upfront to prevent reallocations during deep tree traversals.