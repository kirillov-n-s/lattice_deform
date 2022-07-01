#ifndef LATTICE_RENDERING_OPENGL_MESH_H
#define LATTICE_RENDERING_OPENGL_MESH_H

namespace Lattice{
    class Model;
}

namespace Lattice::Rendering::OpenGL {
    class Mesh
    {
    public:
        Mesh(const Model &model);
        ~Mesh();

        void draw() const;

    private:
        size_t m_nElements;
        unsigned int m_vertexBufferObject,
                     m_vertexArrayObject,
                     m_elementBufferObject;
    };
}

#endif
