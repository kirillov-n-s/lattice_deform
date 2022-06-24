#pragma once
#include <string>

namespace Lattice
{
    class Model;
}

namespace Lattice::ObjIO
{
    Lattice::Model read(const std::string&);
    void write(const Lattice::Model&, const std::string&);
}
