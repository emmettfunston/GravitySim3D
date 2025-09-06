# GravitySim3D - Solar System Simulation

A realistic 3D solar system simulation built with C++ and OpenGL, featuring accurate gravitational physics and beautiful visual rendering.

## Features

- **Realistic Physics**: Implements Newtonian gravitational mechanics with proper orbital dynamics
- **Complete Solar System**: Includes all 8 planets plus the Sun with accurate properties
- **Beautiful Graphics**: Advanced lighting, realistic planet colors, and space environment
- **Interactive Camera**: Full 3D camera controls to explore the simulation
- **Real-time Simulation**: Smooth physics simulation with proper time integration

## Screenshots

The simulation displays a beautiful solar system with:
- Golden Sun at the center
- All 8 planets in their characteristic colors
- Realistic orbital mechanics
- Space environment with starfield background
- Proper lighting and shading

## Technical Details

### Physics Engine
- Newton's law of universal gravitation: F = G × m₁ × m₂ / r²
- Verlet integration for stable numerical simulation
- Elastic collision handling with momentum conservation
- Realistic orbital velocity calculations

### Graphics
- OpenGL 3.3 Core Profile
- Phong lighting model (ambient, diffuse, specular)
- GLM for mathematics
- Custom shaders for planets and background

### Solar System Data
- Accurate planetary masses (relative to Earth)
- Realistic orbital distances (scaled for visibility)
- Authentic planet colors and descriptions
- Proper orbital velocities for stable orbits

## Dependencies

- **OpenGL 3.3+**: Graphics rendering
- **GLFW**: Window management and input
- **GLM**: Mathematics library
- **GLAD**: OpenGL function loader
- **Dear ImGui**: User interface
- **CMake**: Build system

## Building

```bash
mkdir build
cd build
cmake ..
make
./GravitySim3D
```

## Controls

- **WASD**: Move camera
- **Mouse**: Look around
- **Q/E**: Move up/down
- **ESC**: Exit

## Project Structure

```
GravitySim3D/
├── src/                    # Source code
│   ├── main.cpp           # Main application
│   ├── PhysicsEngine.cpp  # Gravitational physics
│   ├── Camera.cpp         # 3D camera system
│   ├── Shader.cpp         # OpenGL shader management
│   ├── Mesh.cpp           # 3D mesh rendering
│   └── ConfigLoader.cpp   # Configuration system
├── shaders/               # GLSL shaders
│   ├── basic.vs.glsl      # Planet vertex shader
│   ├── basic.fs.glsl      # Planet fragment shader
│   ├── background.vs.glsl # Background vertex shader
│   └── background.fs.glsl # Background fragment shader
├── extern/                # External libraries
│   ├── glad/              # OpenGL loader
│   └── imgui/             # GUI library
├── config/                # Configuration files
│   └── simulation.json    # Solar system data
├── CMakeLists.txt         # Build configuration
└── README.md              # This file
```

## Educational Value

This simulation demonstrates:
- Classical mechanics and orbital dynamics
- Real-time physics simulation techniques
- 3D graphics programming with OpenGL
- Numerical integration methods
- Solar system astronomy

## Future Enhancements

- Add moons and asteroid belts
- Implement elliptical orbits
- Add orbital trails
- Include dwarf planets
- Add sound effects
- Multi-body gravitational interactions

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
