#ifndef LATTICE_RENDERING_CONVERSION_H
#define LATTICE_RENDERING_CONVERSION_H

#include <functional>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace Lattice{
    class Model;
}

namespace Lattice::Rendering::Conversion{
    struct Vertex
    {
        glm::vec4 point;
        glm::vec3 texcoord;
        glm::vec3 normal;
    };

    struct Index
    {
        int64_t point;
        int64_t texcoord;
        int64_t normal;
    };

    Index makeIndex(
        const Model &model,
        const size_t flatIndex);
    Vertex vertexAtIndex(
        const Model &model,
        const Index &index);

    bool operator==(
        const Index &lhs,
        const Index &rhs);
    bool operator!=(
        const Index &lhs,
        const Index &rhs);

    bool isTriangle(const Model &model);
    Model triangulate(const Model &model);
}

template<>
struct std::hash<Lattice::Rendering::Conversion::Index>
{
    size_t operator()(const Lattice::Rendering::Conversion::Index &index) const
    {
        return std::hash<int64_t>{}(index.point)
             ^ std::hash<int64_t>{}(index.texcoord)
             ^ std::hash<int64_t>{}(index.normal);
    }
};

#endif
