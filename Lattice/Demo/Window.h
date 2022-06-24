#pragma once
#include <iostream>
#include <string>
#include "../Rendering/Camera.h"

class GLFWwindow;

namespace Lattice::Rendering
{
    class Camera;

    namespace OpenGL
    {
        class Mesh;
        class Shader;
    }
}

namespace Lattice::Demo
{
    class Window
    {
    private:
        static inline GLFWwindow* _window;
        static inline int _width;
        static inline int _height;
        static inline float _aspect;

        static inline Lattice::Rendering::OpenGL::Mesh* _mesh;
        static inline Lattice::Rendering::OpenGL::Shader* _shader;
        static inline Lattice::Rendering::Camera _camera;

        static inline float _x_prev;
        static inline float _y_prev;

        static inline float _sensitivity = 0.1f;
        static inline float _speed = 2.5f;

        static inline float _fov = 45.f;
        static inline float _near = 0.1f;
        static inline float _far = 1000.f;

        static inline glm::mat4 _model = glm::mat4 { 1.f };
        static inline glm::mat4 _normal_model = glm::transpose(glm::inverse(_model));

        static void resize_callback(GLFWwindow*, int, int);
        static void cursor_callback(GLFWwindow*, double, double);
        static void scroll_callback(GLFWwindow*, double, double);
        static void key_callback(GLFWwindow*, int, int, int, int);

        static void handle_movement(float);
        static void render();

    public:
        static void create(int, int);
        static void load(
                Lattice::Rendering::OpenGL::Mesh*,
                Lattice::Rendering::OpenGL::Shader*,
                const Lattice::Rendering::Camera&);
        static void run();
        static void destroy();
    };
}
