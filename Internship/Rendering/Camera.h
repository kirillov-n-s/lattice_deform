#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Internship::Directions
{
    const glm::vec3 ZERO = {0.f, 0.f, 0.f };
    const glm::vec3 RIGHT = {1.f, 0.f, 0.f };
    const glm::vec3 LEFT = {-1.f, 0.f, 0.f };
    const glm::vec3 UP = {0.f, 1.f, 0.f };
    const glm::vec3 DOWN = {0.f, -1.f, 0.f };
    const glm::vec3 BACK = {0.f, 0.f, 1.f };
    const glm::vec3 FRONT = {0.f, 0.f, -1.f };
}

namespace Internship::Rendering
{
    class Camera
    {
    private:
        glm::vec3 _position;
        float _yaw;
        float _pitch;

        glm::vec3 _front;
        glm::vec3 _right;
        glm::vec3 _up;

        void update_derived_vectors();

    public:
        Camera(const glm::vec3& position = Directions::BACK * 3.f, float yaw = -90.f, float pitch = 0.f);

        glm::vec3 front() const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        glm::mat4 view() const;

        void move(const glm::vec3& offset);
        void rotate(float yaw_offset, float pitch_offset);
    };
}
