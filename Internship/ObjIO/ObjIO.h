#pragma once
#include <string>

namespace Internship
{
    class Model;
}

namespace Internship::ObjIO
{
    Internship::Model read(const std::string&);
    void write(const Internship::Model&, const std::string&);
}
