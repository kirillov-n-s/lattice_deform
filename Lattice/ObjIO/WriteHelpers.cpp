#include "WriteHelpers.h"

namespace Lattice::ObjIO::WriteHelpers {
    void writeFaceFull(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &texcoordIndices,
        const std::vector<int64_t> &normalIndices,
        const size_t idx)
    {
        file << (pointIndices[idx] + 1) << '/'
             << (texcoordIndices[idx] + 1) << '/'
             << (normalIndices[idx] + 1) << ' ';
    }

    void writeFaceNoNormals(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &texcoordIndices,
        const std::vector<int64_t> &,
        const size_t idx)
    {
        file << (pointIndices[idx] + 1) << '/'
             << (texcoordIndices[idx] + 1) << ' ';
    }

    void writeFaceNoTexcoords(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &,
        const std::vector<int64_t> &normalIndices,
        const size_t idx)
    {
        file << (pointIndices[idx] + 1) << "//"
             << (normalIndices[idx] + 1) << ' ';
    }

    void writeFaceOnlyPoints(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &,
        const std::vector<int64_t> &,
        const size_t idx)
    {
        file << (pointIndices[idx] + 1) << ' ';
    }

    WriteFaceFunction chooseWriteFaceFunction(
        const bool hasTexcoords,
        const bool hasNormals)
    {
        if (hasTexcoords && hasNormals)
            return writeFaceFull;
        if (hasTexcoords)
            return writeFaceNoNormals;
        if (hasNormals)
            return writeFaceNoTexcoords;
        return writeFaceOnlyPoints;
    }
}
