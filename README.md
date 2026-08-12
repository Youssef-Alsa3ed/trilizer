# trilizer

OpenGL renderer written in C++20.

## Features

- Model loading (OBJ / GLTF via Assimp)
- Phong lighting with directional and point lights
- Normal mapping (with adjustable strength)

- Asynchronous texture loading
- Dear ImGui debug UI

## Showcase

![Showcase](public/showcase.mp4)

## Build

Dependencies (GLFW, GLM, Assimp, ImGui, GLAD, stb) are git submodules.

```bash
git clone --recurse-submodules <repo-url>
cmake -B build
cmake --build build
./build/bin/trilizer
```

## Controls

- `WASD` — move camera
- Mouse — look around (hold right mouse button to capture the cursor)
