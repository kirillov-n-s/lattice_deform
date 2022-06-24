#include "Mesh.h"
#include "../Conversion.h"
#include "../../Model.h"

namespace Lattice::Rendering::OpenGL
{
    Mesh::Mesh(const Model &model)
    {
        const auto& triModel = Conversion::isTriangle(model)
                               ? model
                               : Conversion::triangulate(model);

        std::vector<Conversion::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::unordered_map<Conversion::Index, unsigned int> indexMap;

        auto nIndices = triModel.pointIndices().size();
        for (size_t i = 0; i < nIndices; i++)
        {
            const auto& index = Conversion::makeIndex(triModel, i);
            auto iter = indexMap.find(index);
            if (iter != indexMap.end())
                indices.push_back(iter->second);
            else
            {
                auto mappedIndex = indexMap.size();
                indexMap[index] = mappedIndex;
                indices.push_back(mappedIndex);
                vertices.push_back(vertexAtIndex(triModel, index));
            }
        }

        _element_count = indices.size();

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Conversion::Vertex),
            vertices.data(),
            GL_STATIC_DRAW);

        glVertexAttribPointer(
            0,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Conversion::Vertex),
            nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Conversion::Vertex),
            (void*)offsetof(Conversion::Vertex, texcoord));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            2,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Conversion::Vertex),
            (void*)offsetof(Conversion::Vertex, normal));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _element_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);
    }

    void Mesh::draw() const
    {
        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, _element_count, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
}
