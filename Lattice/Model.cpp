#include "Model.h"
#include <algorithm>

namespace Lattice {
    Model::Model(
        const Points &points,
        const Texcoords &texcoords,
        const Normals &normals,
        const Indices &point_indices,
        const Indices &texcoord_indices,
        const Indices &normal_indices,
        const FaceSizes &face_sizes)
        : m_points(points),
          m_pointIndices(point_indices),
          _face_sizes(face_sizes)
    {
        if (!texcoords.empty() && !texcoord_indices.empty()) {
            m_texcoords = texcoords;
            m_texcoordIndices = texcoord_indices;
        }

        if (!normals.empty() && !normal_indices.empty()) {
            m_normals = normals;
            m_normalIndices = normal_indices;
        }
    }

    const Model::Points &Model::points() const
    {
        return m_points;
    }

    const Model::Texcoords &Model::texcoords() const
    {
        return m_texcoords;
    }

    const Model::Normals &Model::normals() const
    {
        return m_normals;
    }

    const Model::Indices &Model::pointIndices() const
    {
        return m_pointIndices;
    }

    const Model::Indices &Model::texcoordIndices() const
    {
        return m_texcoordIndices;
    }

    const Model::Indices &Model::normalIndices() const
    {
        return m_normalIndices;
    }

    const Model::FaceSizes &Model::faceSizes() const
    {
        return _face_sizes;
    }

    bool Model::hasTexcoords() const
    {
        return !m_texcoords.empty();
    }

    bool Model::hasNormals() const
    {
        return !m_normals.empty();
    }
}
