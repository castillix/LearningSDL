# 3D Voxel Zombie Survival Engine

Welcome to the 3D Voxel Zombie Survival Engine! This project is a custom-built, lightweight 3D game engine constructed from scratch using **C++17**, **SDL3**, **OpenGL 3.3 Core**, and **ImGui**. 

It is currently in **Phase 2.5** of development, establishing the core gameplay loop of classic zombie survival within a voxel environment, with modular entities, dynamic map loading, and high-performance AI pathfinding.

## Features & Architecture Philosophy

The engine is split into two primary modules: the `Engine` static library, and the `Zombies` game executable. This modularity ensures that the underlying rendering and physics tech is completely decoupled from the specific gameplay logic.

### 1. Rendering (OpenGL & Voxel Meshing)
Rendering is handled efficiently via raw OpenGL buffer uploads (`VAO`s and `VBO`s).
- **VoxelMap & Chunks**: The world is divided into 32x32x32 `Chunk` objects managed by the `VoxelMap`. 
- **Surface Culling**: The `Chunk` class performs surface culling—it only generates geometry faces that border non-solid blocks (like Air). The winding order is correctly configured to `GL_CCW` to prevent inside-out rendering.
- **ImGui Integration**: A transparent debug overlay provides real-time tracking of FPS, Frame Time, Player Coordinates, and Game Score.

### 2. Physics & Interactions
- **3D DDA Raycasting**: The `VoxelRaycaster` class uses the Fast Voxel Traversal algorithm (DDA) to instantly calculate exactly which blocks a 3D line intersects. This is utilized by the `Weapon` class for hitscan detection against zombies and walls.
- **AABB Collision**: Entities (`Player`, `Zombie`) sweep Axis-Aligned Bounding Boxes against the `VoxelMap`, utilizing `IMap::isSolid()` to correctly map bounds against standard structural blocks.

### 3. AI & Navigation (3D Flow Fields)
- **Vector Fields**: The `FlowField` class maps a `Dijkstra` flood-fill field originating from the Player.
- **Vertical Navigation**: The search kernel extends across the Y-axis, allowing paths to be generated up and over obstacles. 
- **Auto-Vaulting**: Zombies automatically read the Flowfield vector. If a path dictates they move forward through a short `VoxelMap` obstruction (like a 1-block high window), the Zombie physics controller applies vertical impulse physics to leap over it.

### 4. Current Map State (`TestingState`)
The game launches directly into `TestingState`, consisting of a 64x64 mapped arena.
- **Rooms & Spawners**: The map features "Room 1" and "Room 2", separated by a purchasable Door.
- **Interactions**: Walking up to the door and pressing `E` deducts 500 Score from the player, unloads the door blocks, and activates the Zombie spawners outside Room 2, causing them to breach through the windows.

## How to Build
This project utilizes **CMake** for build-system generation and **vcpkg** for dependency acquisition.

### Prerequisites:
- CMake 3.16+
- vcpkg (with the `vcpkg.cmake` toolchain)
- A modern C++ compiler (MSVC, MinGW, or GCC/Clang)

### Compilation Steps:
Run the following from the root directory in your terminal:
```bash
# Generate the build files
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake"

# Build the project
cmake --build build
```

## How to Run & Play
Once compiled, you can launch the game via the generated executable:
`bin/Debug-x64/Zombies.exe` (or `Release-x64`).

### Controls
- **W/A/S/D**: Move
- **Space**: Jump
- **Mouse**: Look around
- **Left Click**: Fire weapon (Raycast hit detection)
- **E**: Interact (Purchase Door to Room 2)
- **Escape**: Quit game

*(Engine explicitly built to demonstrate the capabilities of high-performance C++ and custom OpenGL context manipulation).*
