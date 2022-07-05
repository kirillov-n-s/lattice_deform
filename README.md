Recreation of Houdini lattice node with OpenGL rendering and minimal unit testing.

Requirements:
- `OpenGL 3.3`
- `glfw` window library
- `glew` function loading library
- `glm` math library
- `Catch2 3` unit testing framework

Warning: mesh files used in `main.cpp` are not included.

Another warning: by default, demo works only in release mode and tests work only in debug mode. That is because I do not know how to make conditional compilation definitions with CMake and `Catch2` does not compile in release mode for some reason.
