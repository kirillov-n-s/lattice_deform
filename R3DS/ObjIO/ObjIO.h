#pragma once
#include <string>

namespace R3DS
{
    class Model;
}

namespace R3DS::ObjIO
{
    R3DS::Model read(const std::string&);
    void write(const R3DS::Model&, const std::string&);
}
