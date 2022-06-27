#ifndef LATTICE_MODEL_H
#define LATTICE_MODEL_H

#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace Lattice{
    class Model
    {
    public:
        using Points = std::vector<glm::vec4>;
        using Texcoords = std::vector<glm::vec3>;
        using Normals = std::vector<glm::vec3>;
        using Indices = std::vector<int64_t>;
        using FaceSizes = std::vector<size_t>;

        Model(
            const Points &points,
            const Texcoords &texcoords,
            const Normals &normals,
            const Indices &pointIndices,
            const Indices &texcoordIndices,
            const Indices &normalIndices,
            const FaceSizes &faceSizes);

        const Points &points() const;
        const Texcoords &texcoords() const;
        const Normals &normals() const;

        const Indices &pointIndices() const;
        const Indices &texcoordIndices() const;
        const Indices &normalIndices() const;

        const FaceSizes &faceSizes() const;

        bool hasTexcoords() const;
        bool hasNormals() const;

    private:
        Points m_points;
        Texcoords m_texcoords;
        Normals m_normals;

        Indices m_pointIndices;
        Indices m_texcoordIndices;
        Indices m_normalIndices;

        FaceSizes _face_sizes;
    };
}

#endif
