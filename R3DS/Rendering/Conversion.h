#pragma once
#include <functional>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace R3DS
{
    class Model;
}

namespace R3DS::Rendering::Conversion
{
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

    Index makeIndex(const Model&, size_t);
    Vertex vertexAtIndex(const Model&, const Index&);

    bool operator==(const Index&, const Index&);
    bool operator!=(const Index&, const Index&);

    bool isTriangle(const Model&);
    Model triangulate(const Model&);
}

template<>
struct std::hash<R3DS::Rendering::Conversion::Index>
{
    size_t operator()(const R3DS::Rendering::Conversion::Index &index) const
    {
        auto hp = std::hash<int64_t>{}(index.point);
        auto ht = std::hash<int64_t>{}(index.texcoord);
        auto hn = std::hash<int64_t>{}(index.normal);
        return hp ^ ht ^ hn;
    }
};
