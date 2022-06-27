#ifndef LATTICE_OBJIO_READHELPERS_H
#define LATTICE_OBJIO_READHELPERS_H

#include <fstream>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace Lattice::ObjIO::ReadHelpers {
    glm::vec4 readPoint(std::ifstream &file);
    glm::vec3 readTexcoord(std::ifstream &file);
    glm::vec3 readNormal(std::ifstream &file);
}

#endif
