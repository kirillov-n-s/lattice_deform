#pragma once
#include <vector>
#include <unordered_map>
#include <GL/glew.h>

namespace Lattice
{
    class Model;
}

namespace Lattice::Rendering::OpenGL
{
    constexpr float PI = 3.14159265f;

    class Mesh
    {
    private:
        int _element_count;
        unsigned int _vbo, _vao, _ebo;

    public:
        Mesh(const Model&);
        ~Mesh();

        void draw() const;
    };
}