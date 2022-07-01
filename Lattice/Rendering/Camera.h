#ifndef LATTICE_RENDERING_CAMERA_H
#define LATTICE_RENDERING_CAMERA_H

#include <glm/glm.hpp>

namespace Lattice::Directions {
    const glm::vec3 ZERO = {0.f, 0.f, 0.f };
    const glm::vec3 RIGHT = {1.f, 0.f, 0.f };
    const glm::vec3 LEFT = {-1.f, 0.f, 0.f };
    const glm::vec3 UP = {0.f, 1.f, 0.f };
    const glm::vec3 DOWN = {0.f, -1.f, 0.f };
    const glm::vec3 BACK = {0.f, 0.f, 1.f };
    const glm::vec3 FRONT = {0.f, 0.f, -1.f };
}

namespace Lattice::Rendering {
    class Camera
    {
    public:
        Camera(
            const glm::vec3 &position = Directions::BACK * 3.f,
            const float yaw = -90.f,
            const float pitch = 0.f);

        glm::vec3 front() const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        glm::mat4 view() const;

        void move(const glm::vec3& offset);
        void rotate(
            const float yawOffset,
            const float pitchOffset);

    private:
        glm::vec3 m_position;
        float m_yaw;
        float m_pitch;

        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec3 m_up;

        void updateDerivedVectors();
    };
}

#endif
