#include "BoundingBox.h"
#include "../Model.h"

namespace Lattice
{
    glm::vec3 BoundingBox::diag() const
    {
        return max - min;
    }

    bool BoundingBox::contains(const glm::vec3 &point) const
    {
        return
            point.x >= min.x && point.y >= min.y && point.z >= min.z &&
            point.x <= max.x && point.y <= max.y && point.z <= max.z;
    }

    bool BoundingBox::contains(const BoundingBox &bbox) const
    {
        return contains(bbox.min) && contains(bbox.max);
    }

    bool BoundingBox::overlaps(const BoundingBox &bbox) const
    {
        return contains(bbox.min) || contains(bbox.max);
    }

    BoundingBox makeBoundingBox(const Model &model)
    {
        glm::vec3 min = glm::vec3 { std::numeric_limits<float>::max() },
                  max = glm::vec3 { std::numeric_limits<float>::lowest() };
        const auto& points = model.points();
        for (const auto& point : points)
        {
            if (point.x < min.x)
                min.x = point.x;
            if (point.x > max.x)
                max.x = point.x;

            if (point.y < min.y)
                min.y = point.y;
            if (point.y > max.y)
                max.y = point.y;

            if (point.z < min.z)
                min.z = point.z;
            if (point.z > max.z)
                max.z = point.z;
        }
        return { min, max };
    }
}
