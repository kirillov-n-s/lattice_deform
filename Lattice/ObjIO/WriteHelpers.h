#ifndef LATTICE_OBJIO_WRITEHELPERS_H
#define LATTICE_OBJIO_WRITEHELPERS_H

#include <vector>
#include <fstream>

namespace Lattice::ObjIO::WriteHelpers {
    void writeFaceFull(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &texcoordIndices,
        const std::vector<int64_t> &normalIndices,
        const size_t idx);
    void writeFaceNoNormals(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &texcoordIndices,
        const std::vector<int64_t> &normalIndices,
        const size_t idx);
    void writeFaceNoTexcoords(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &texcoordIndices,
        const std::vector<int64_t> &normalIndices,
        const size_t idx);
    void writeFaceOnlyPoints(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &texcoordIndices,
        const std::vector<int64_t> &normalIndices,
        const size_t idx);

    using WriteFaceFunction = void (*) (
        std::ofstream&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        const size_t);
    WriteFaceFunction chooseWriteFaceFunction(
        const bool hasTexcoords,
        const bool hasNormals);
}

#endif
