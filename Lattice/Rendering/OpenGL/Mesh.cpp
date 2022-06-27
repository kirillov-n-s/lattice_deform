#include "Mesh.h"
#include "../Conversion.h"
#include "../../Model.h"
#include <GL/glew.h>
#include <vector>
#include <unordered_map>

namespace Lattice::Rendering::OpenGL{
    Mesh::Mesh(const Model &model)
    {
        const auto& triangleModel = Conversion::isTriangle(model)
                                    ? model
                                    : Conversion::triangulate(model);

        std::vector<Conversion::Vertex> vertices;
        std::vector<unsigned int> flatIndices;
        std::unordered_map<Conversion::Index, unsigned int> indexMap;

        const auto nPointIndices = triangleModel.pointIndices().size();
        for (size_t pointIdx = 0; pointIdx < nPointIndices; pointIdx++) {
            const auto& convIdx = Conversion::makeIndex(triangleModel, pointIdx);
            const auto& indexPairIt = indexMap.find(convIdx);
            if (indexPairIt != indexMap.end()) {
                flatIndices.push_back(indexPairIt->second);
                continue;
            }
            const size_t flatIdx = indexMap.size();
            indexMap[convIdx] = flatIdx;
            flatIndices.push_back(flatIdx);
            vertices.push_back(vertexAtIndex(triangleModel, convIdx));
        }

        m_nElems = flatIndices.size();

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
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

        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            m_nElems * sizeof(unsigned int),
            flatIndices.data(),
            GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }

    void Mesh::draw() const
    {
        glBindVertexArray(m_vao);
        glDrawElements(
            GL_TRIANGLES,
            m_nElems,
            GL_UNSIGNED_INT,
            nullptr);
        glBindVertexArray(0);
    }
}
