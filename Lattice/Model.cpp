#include "Model.h"
#include <algorithm>

namespace Lattice {
    Model::Model(
        const Points &points,
        const Indices &pointIndices,
        const FaceSizes &faceSizes)
        : m_points(points),
          m_pointIndices(pointIndices),
          _face_sizes(faceSizes)
    {}

    Model::Model(
        const Points &points,
        const Texcoords &texcoords,
        const Normals &normals,
        const Indices &pointIndices,
        const Indices &texcoordIndices,
        const Indices &normalIndices,
        const FaceSizes &faceSizes)
        : Model(points, pointIndices, faceSizes)
    {
        if (!texcoords.empty() && !texcoordIndices.empty()) {
            m_texcoords = texcoords;
            m_texcoordIndices = texcoordIndices;
        }

        if (!normals.empty() && !normalIndices.empty()) {
            m_normals = normals;
            m_normalIndices = normalIndices;
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

    bool operator==(
        const Model &lhs,
        const Model &rhs)
    {
        return lhs.points() == rhs.points()
            && lhs.texcoords() == rhs.texcoords()
            && lhs.normals() == rhs.normals()
            && lhs.pointIndices() == rhs.pointIndices()
            && lhs.texcoordIndices() == rhs.texcoordIndices()
            && lhs.normalIndices() == rhs.normalIndices()
            && lhs.faceSizes() == rhs.faceSizes();
    }

    bool operator!=(
        const Model &lhs,
        const Model &rhs)
    {
        return !(lhs == rhs);
    }
}
