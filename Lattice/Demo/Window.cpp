#include "Window.h"
#include <string>
#include "../Rendering/OpenGL/Mesh.h"
#include "../Rendering/OpenGL/Shader.h"
#include <GLFW/glfw3.h>

namespace Lattice::Demo {
    void Window::resizeCallback(
        GLFWwindow*,
        int newWidth,
        int newHeight)
    {
        glViewport(
            0,
            0,
            s_width = newWidth,
            s_height = newHeight);
    }

    void Window::cursorCallback(
        GLFWwindow*,
        double xPos,
        double yPos)
    {
        float x_offset = xPos - s_xPrevPos;
        float y_offset = s_yPrevPos - yPos;
        s_xPrevPos = xPos;
        s_yPrevPos = yPos;
        s_camera.rotate(
            x_offset * s_sensitivity,
            y_offset * s_sensitivity);
    }

    void Window::scrollCallback(
        GLFWwindow*,
        double,
        double yOffset)
    {
        if ((s_speed += yOffset) < 0.f)
            s_speed = 0.f;
    }

    void Window::keyCallback(
        GLFWwindow*,
        int key,
        int,
        int action,
        int)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(s_window, GLFW_TRUE);
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            glfwMaximizeWindow(s_window);
        if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
            s_camera = {};
    }

    void Window::handleMovement(const float frameTime)
    {
        glm::vec3 direction = Lattice::Directions::ZERO;
        if (glfwGetKey(s_window, GLFW_KEY_W) == GLFW_PRESS)
            direction += s_camera.front();
        if (glfwGetKey(s_window, GLFW_KEY_S) == GLFW_PRESS)
            direction -= s_camera.front();
        if (glfwGetKey(s_window, GLFW_KEY_D) == GLFW_PRESS)
            direction += s_camera.right();
        if (glfwGetKey(s_window, GLFW_KEY_A) == GLFW_PRESS)
            direction -= s_camera.right();
        if (glfwGetKey(s_window, GLFW_KEY_Q) == GLFW_PRESS)
            direction += Lattice::Directions::UP;
        if (glfwGetKey(s_window, GLFW_KEY_E) == GLFW_PRESS)
            direction -= Lattice::Directions::UP;
        s_camera.move(direction * frameTime * s_speed);
    }

    void Window::render()
    {
        s_shader->use();
        s_shader->setUniform("uView", s_camera.view());
        s_shader->setUniform("uCameraDir", s_camera.front());

        glPolygonOffset(1,1);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        s_shader->setUniform(
                "uColor",
                glm::vec4{0.75f, 0.75f, 0.75f, 1.f});
        s_mesh->draw();

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        s_shader->setUniform(
                "uColor",
                glm::vec4{0.5f, 0.5f, 0.5f, 1.f});
        s_mesh->draw();
    }

    void Window::create(int width, int height)
    {
        s_width = width;
        s_height = height;
        s_aspect = (float)s_width / s_height;

        glfwInit();
        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MAJOR,
            4);
        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MINOR,
            6);
        glfwWindowHint(
            GLFW_OPENGL_PROFILE,
            GLFW_OPENGL_CORE_PROFILE);

        s_window = glfwCreateWindow(
            s_width,
            s_height,
            "",
            nullptr,
            nullptr);
        if (s_window == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Cannot open window.");
        }

        glfwMakeContextCurrent(s_window);

        glfwSetInputMode(
            s_window,
            GLFW_CURSOR,
            GLFW_CURSOR_DISABLED);

        glfwSetFramebufferSizeCallback(s_window, resizeCallback);
        glfwSetCursorPosCallback(s_window, cursorCallback);
        glfwSetScrollCallback(s_window, scrollCallback);
        glfwSetKeyCallback(s_window, keyCallback);

        glViewport(
            0,
            0,
            s_width,
            s_height);

        glewInit();
    }

    void Window::load(
        Lattice::Rendering::OpenGL::Mesh *mesh,
        Lattice::Rendering::OpenGL::Shader *shader,
        const Lattice::Rendering::Camera &camera)
    {
        s_mesh = mesh;
        s_shader = shader;
        s_camera = camera;
    }

    void Window::run()
    {
        float currentFrameElapsed,
              prevFrameElapsed = 0.f,
              frameTime;

        s_shader->use();
        s_shader->setUniform(
                "uProjection",
                glm::perspective(
                        glm::radians(s_fov),
                        s_aspect,
                        s_near,
                        s_far));
        s_shader->setUniform("uModel", s_model);
        s_shader->setUniform("uNormalModel", s_normalModel);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glEnable(GL_DEPTH_TEST);

        while(!glfwWindowShouldClose(s_window))
        {
            currentFrameElapsed = glfwGetTime();
            frameTime = currentFrameElapsed - prevFrameElapsed;
            prevFrameElapsed = currentFrameElapsed;

            glfwSetWindowTitle(
                s_window,
                (std::to_string((int)(1.f / frameTime)) + " fps").c_str());

            glClearColor(0.15f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            handleMovement(frameTime);
            render();

            glfwSwapBuffers(s_window);
            glfwPollEvents();
        }
    }

    void Window::destroy()
    {
        delete s_mesh;
        delete s_shader;
        glfwTerminate();
    }
}
