#ifndef LATTICE_DEMO_WINDOW_H
#define LATTICE_DEMO_WINDOW_H

#include <iostream>
#include "../Rendering/Camera.h"

class GLFWwindow;

namespace Lattice::Rendering {
    class Camera;

    namespace OpenGL {
        class Mesh;
        class Shader;
    }
}

namespace Lattice::Demo {
    class Window
    {
    public:
        static void create(int width, int height);
        static void load(
            Lattice::Rendering::OpenGL::Mesh *mesh,
            Lattice::Rendering::OpenGL::Shader *shader,
            const Lattice::Rendering::Camera &camera);
        static void run();
        static void destroy();

    private:
        static inline GLFWwindow* s_window;
        static inline int s_width;
        static inline int s_height;
        static inline float s_aspect;

        static inline Lattice::Rendering::OpenGL::Mesh* s_mesh;
        static inline Lattice::Rendering::OpenGL::Shader* s_shader;
        static inline Lattice::Rendering::Camera s_camera;

        static inline float s_xPrevPos;
        static inline float s_yPrevPos;

        static inline const float s_sensitivity = 0.1f;
        static inline float s_speed = 2.5f;

        static inline const float s_fov = 45.f;
        static inline const float s_near = 0.1f;
        static inline const float s_far = 1000.f;

        static inline const glm::mat4 s_model = glm::mat4 {1.f };
        static inline const glm::mat4 s_normalModel = glm::transpose(glm::inverse(s_model));

        static void resizeCallback(
            GLFWwindow*,
            int newWidth,
            int newHeight);
        static void cursorCallback(
            GLFWwindow*,
            double xPos,
            double yPos);
        static void scrollCallback(
            GLFWwindow*,
            double,
            double yOffset);
        static void keyCallback(
            GLFWwindow*,
            int key,
            int,
            int action,
            int);

        static void handleMovement(const float frameTime);
        static void render();
    };
}

#endif
