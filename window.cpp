#include "window.h"

void window::resize_callback(GLFWwindow *window, int new_width, int new_height)
{
    _width = new_width;
    _height = new_height;
    glViewport(0, 0, _width, _height);
}

void window::cursor_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    float x_offset = x_pos - _x_prev;
    float y_offset = _y_prev - y_pos;
    _x_prev = x_pos;
    _y_prev = y_pos;
    _camera.rotate(x_offset * _sensitivity, y_offset * _sensitivity);
}

void window::scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
    if ((_speed += y_offset) < 0.f)
        _speed = 0.f;
}

void window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, GLFW_TRUE);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        glfwMaximizeWindow(_window);

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        _camera = {};

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, (_wireframe ^= true) ? GL_LINE : GL_FILL);
}

void window::handle_movement(float frame_time)
{
    glm::vec3 direction = R3DS::Directions::ZERO;
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        direction += _camera.front();
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        direction -= _camera.front();
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        direction += _camera.right();
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        direction -= _camera.right();
    if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
        direction += R3DS::Directions::UP;
    if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
        direction -= R3DS::Directions::UP;
    _camera.move(direction * frame_time * _speed);
}

void window::render()
{
    _shader->use();
    _shader->set_uniform("uView", _camera.view());
    _shader->set_uniform("uCameraDir", _camera.front());

    glPolygonOffset(1,1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    _shader->set_uniform("uColor", glm::vec4 { 0.75f, 0.75f, 0.75f, 1.f });
    _mesh->draw();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDisable(GL_POLYGON_OFFSET_FILL);
    _shader->set_uniform("uColor", glm::vec4 { 0.5f, 0.5f, 0.5f, 1.f });
    _mesh->draw();
}

void window::create(int width, int height)
{
    _width = width;
    _height = height;
    _aspect = (float)_width / _height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_width, _height, "", nullptr, nullptr);
    if (_window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Cannot open window.");
    }

    glfwMakeContextCurrent(_window);

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(_window, resize_callback);
    glfwSetCursorPosCallback(_window, cursor_callback);
    glfwSetScrollCallback(_window, scroll_callback);
    glfwSetKeyCallback(_window, key_callback);

    glViewport(0, 0, _width, _height);

    glewInit();
}

void window::load(
    R3DS::Rendering::OpenGL::Mesh *mesh,
    R3DS::Rendering::OpenGL::Shader *shader,
    const R3DS::Rendering::Camera &camera)
{
    _mesh = mesh;
    _shader = shader;
    _camera = camera;
}

void window::run()
{
    float current_frame, prev_frame = 0.f, frame_time;

    _shader->use();
    _shader->set_uniform("uProjection",
                         glm::perspective(glm::radians(_fov), _aspect, _near, _far));
    _shader->set_uniform("uModel", _model);
    _shader->set_uniform("uNormalModel", _normal_model);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(_window))
    {
        current_frame = glfwGetTime();
        frame_time = current_frame - prev_frame;
        prev_frame = current_frame;

        glfwSetWindowTitle(_window, (std::to_string((int)(1.f / frame_time)) + " fps").c_str());

        glClearColor(0.15f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        handle_movement(frame_time);
        render();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void window::destroy()
{
    delete _mesh;
    delete _shader;
    glfwTerminate();
}
