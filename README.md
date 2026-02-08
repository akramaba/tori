#### If interested, please see the changelog folder for commit notes.

# Tori - 3D Engine in C++

## About

Tori is a custom 3D Engine programmed in C++.

Research for Tori started in July 2025. It was originally a D3D11 project, but then switched to OpenGL for potential cross-platform applications. The majority of the code was made in December 2025 and January 2026 after a long focus on my operating system (Mira OS) from September 2025 to November 2025. Tori was prepared and uploaded to the public on 2/7/2026.

Tori's primary goal is to be as optimized as possible for speed. My development will start to pinpoint where code can further be faster. So far, I've done unrolling, inling of vector math, avoiding reallocation, static arrays, and batched GPU uniform uploads.

I also focused on having a simple API while being feature-rich. Tori's API design achieves this by making the common case (window initialization, user input, MP3 audio playing, camera movement, skeletons animations, engine rendering, etc.) almost trivial through a global state, implicit resource management, hierarchical scenes, and custom math.

## Plans

- Full documentation and more comments. I was glad to start a custom 3D engine after working with 2D graphics since December 2021. I'm looking to add more files like this to document the project structure and design, and comment more in the source files.
- Optimizing code. There's definitely a lot of room for this, especially when it comes to anything compiler-related. Of course, personal code audits will be important for both speed and robustness, as Tori is heavy on math (Math.hpp & Model.cpp).
- Add UI. The code is in progress (supports images, text, and rectangles), but it needs further testing.
- Add more features. Tori's goal is to be a fast and simple API for creating 3D games and scenes, yet still powerful.
- Add benchmarks and more sample applications. This is helpful for when a speed optimization is complete or if a feature needs testing.