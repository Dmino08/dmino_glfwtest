# GLFW Test Project

This project is a simple playground to experiment with OpenGL and related technologies. The long-term goal is to develop a custom game engine, but for now, the focus is on learning and experimenting with core libraries such as OpenGL, Assimp, and others.

## Overview

At this stage, the project includes a few basic but essential components:

- `Window` — handles window creation and OpenGL context
- `Camera` — simple camera implementation
- `Texture` — texture loading and management
- `Shader` — basic shader wrapper
- `Mesh` — basic mesh abstraction
- `Logger` — minimal logging system
- `InputManager` — handles user input

This is still a work in progress and will grow over time as I explore more systems and techniques.

---

## Dependencies

This project uses the following libraries:

- stb
- OpenGL
- glad
- GLM
- Assimp

Dependencies are managed using [vcpkg](https://github.com/microsoft/vcpkg).

### Installing with vcpkg

If you don’t have `vcpkg` yet, clone and bootstrap it (see vcpkg docs). Then, from PowerShell or your terminal:

```powershell
$env:VCPKG_ROOT = "path/to/vcpkg"
cmake --preset debug # inside happening vcpkg install