#pragma once
#include <vector>
#include <algorithm>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace R3DS
{
    class Model
    {
    public:
        using PointIterator = std::vector<glm::vec4>::const_iterator;

    private:
        std::vector<glm::vec4> _points;
        std::vector<glm::vec3> _texcoords;
        std::vector<glm::vec3> _normals;

        std::vector<int64_t> _point_indices;
        std::vector<int64_t> _texcoord_indices;
        std::vector<int64_t> _normal_indices;

        std::vector<int64_t> _face_sizes;

    public:
        Model(const std::vector<glm::vec4>&, const std::vector<glm::vec3>&, const std::vector<glm::vec3>&,
              const std::vector<int64_t>&, const std::vector<int64_t>&, const std::vector<int64_t>&,
              const std::vector<int64_t>&);

        const std::vector<glm::vec4>& points() const;
        const std::vector<glm::vec3>& texcoords() const;
        const std::vector<glm::vec3>& normals() const;

        const std::vector<int64_t>& pointIndices() const;
        const std::vector<int64_t>& texcoordIndices() const;
        const std::vector<int64_t>& normalIndices() const;

        const std::vector<int64_t>& nPointsByFace() const;

        bool hasTexcoords() const;
        bool hasNormals() const;
    };
}
