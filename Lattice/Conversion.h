#ifndef LATTICE_CONVERSION_H
#define LATTICE_CONVERSION_H

#include <utility>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace Lattice {
    class Model;
}

namespace Lattice {
    struct Vertex
    {
        glm::vec4 point;
        glm::vec3 texcoord = glm::vec3 { 0 };
        glm::vec3 normal = glm::vec3 { 0 };
    };

    struct Index
    {
        int64_t point;
        int64_t texcoord = -1;
        int64_t normal = -1;
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

    bool operator==(
        const Vertex &lhs,
        const Vertex &rhs);
    bool operator!=(
        const Vertex &lhs,
        const Vertex &rhs);

    bool isTriangle(const Model &model);
    Model triangulate(const Model &model);

    std::pair<std::vector<Vertex>, std::vector<unsigned int>> indexedVertices(const Model &model);
}

#endif
