#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace R3DS::ObjIO::ReadHelpers
{
    glm::vec4 readPoint(std::ifstream&);
    glm::vec3 readTexcoord(std::ifstream&);
    glm::vec3 readNormal(std::ifstream&);
}
