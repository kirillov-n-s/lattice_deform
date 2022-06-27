#ifndef LATTICE_DEFORM_BOUNDINGBOX_H
#define LATTICE_DEFORM_BOUNDINGBOX_H

#include <glm/vec3.hpp>
#include <glm/common.hpp>

namespace Lattice {
    struct BoundingBox
    {
        glm::vec3 min, max;

        glm::vec3 diag() const;

        bool contains(const glm::vec3 &point) const;
        bool overlaps(const BoundingBox &boundingBox) const;
    };

    class Model;
    BoundingBox makeBoundingBox(const Model &model);
}

#endif
