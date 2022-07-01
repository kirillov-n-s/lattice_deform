#include "Mesh.h"
#include "../Conversion.h"
#include "../../Model.h"
#include <GL/glew.h>
#include <vector>
#include <unordered_map>

namespace Lattice::Rendering::OpenGL {
    Mesh::Mesh(const Model &model)
    {
        const auto& [vertices, indices] = Conversion::indexedVertices(model);
        m_nElements = indices.size();

        glGenVertexArrays(1, &m_vertexArrayObject);
        glBindVertexArray(m_vertexArrayObject);

        glGenBuffers(1, &m_vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
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

        glGenBuffers(1, &m_elementBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
        glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                m_nElements * sizeof(unsigned int),
                indices.data(),
                GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
        glDeleteBuffers(1, &m_vertexBufferObject);
        glDeleteBuffers(1, &m_elementBufferObject);
    }

    void Mesh::draw() const
    {
        glBindVertexArray(m_vertexArrayObject);
        glDrawElements(
                GL_TRIANGLES,
                m_nElements,
                GL_UNSIGNED_INT,
                nullptr);
        glBindVertexArray(0);
    }
}
