#include "ReadHelpers.h"

namespace Lattice::ObjIO::ReadHelpers
{
    glm::vec4 readPoint(std::ifstream &file)
    {
        glm::vec4 vertex;
        file >> vertex.x >> vertex.y >> vertex.z;
        if (!file)
            throw std::runtime_error("Bad vertex_t coordinates.");
        file >> vertex.w;
        if (!file) {
            file.clear();
            vertex.w = 1.f;
        }
        return vertex;
    }

    glm::vec3 readTexcoord(std::ifstream &file)
    {
        glm::vec3 texcoord;
        file >> texcoord.x >> texcoord.y;
        if (!file)
            throw std::runtime_error("Bad texture coordinates.");
        file >> texcoord.z;
        if (!file) {
            file.clear();
            texcoord.z = 0.f;
        }
        return texcoord;
    }


    glm::vec3 readNormal(std::ifstream &file)
    {
        glm::vec3 normal;
        file >> normal.x >> normal.y >> normal.z;
        if (!file)
            throw std::runtime_error("Bad normal coordinates.");
        return normal;
    }
}
