#ifndef LATTICE_OBJIO_OBJIO_H
#define LATTICE_OBJIO_OBJIO_H

#include <string>

namespace Lattice {
    class Model;
}

namespace Lattice::ObjIO {
    Lattice::Model read(const std::string &path);
    void write(
        const Lattice::Model &model,
        const std::string &path);
}

#endif
