#include "Model.h"

namespace R3DS
{
    Model::Model(const std::vector<glm::vec4> &points,
                 const std::vector<glm::vec3> &texcoords,
                 const std::vector<glm::vec3> &normals,
                 const std::vector<int64_t> &point_indices,
                 const std::vector<int64_t> &texcoord_indices,
                 const std::vector<int64_t> &normal_indices,
                 const std::vector<int64_t> &face_sizes)
        : _points(points), _point_indices(point_indices), _face_sizes(face_sizes)
    {
        if (!texcoords.empty() && !texcoord_indices.empty())
        {
            _texcoords = texcoords;
            _texcoord_indices = texcoord_indices;
        }

        if (!normals.empty() && !normal_indices.empty())
        {
            _normals = normals;
            _normal_indices = normal_indices;
        }
    }

    const std::vector<glm::vec4> &Model::points() const
    {
        return _points;
    }

    const std::vector<glm::vec3> &Model::texcoords() const
    {
        return _texcoords;
    }

    const std::vector<glm::vec3> &Model::normals() const
    {
        return _normals;
    }

    const std::vector<int64_t> &Model::pointIndices() const
    {
        return _point_indices;
    }

    const std::vector<int64_t> &Model::texcoordIndices() const
    {
        return _texcoord_indices;
    }

    const std::vector<int64_t> &Model::normalIndices() const
    {
        return _normal_indices;
    }

    const std::vector<int64_t> &Model::nPointsByFace() const
    {
        return _face_sizes;
    }

    bool Model::hasTexcoords() const
    {
        return !_texcoords.empty();
    }

    bool Model::hasNormals() const
    {
        return !_normals.empty();
    }
}
