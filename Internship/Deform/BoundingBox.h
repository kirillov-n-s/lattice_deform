#pragma once
#include <glm/vec3.hpp>
#include <glm/common.hpp>

namespace Internship
{
    struct BoundingBox
    {
        glm::vec3 min, max;

        glm::vec3 diag() const;

        bool contains(const glm::vec3&) const;
        bool contains(const BoundingBox&) const;
        bool overlaps(const BoundingBox&) const;
    };

    class Model;
    BoundingBox makeBoundingBox(const Model&);
}
