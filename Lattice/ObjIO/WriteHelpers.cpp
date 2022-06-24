#include "WriteHelpers.h"

namespace Lattice::ObjIO::WriteHelpers
{
    void writeFaceFull(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &texcoordIndices,
        const std::vector<int64_t> &normalIndices,
        int64_t index)
    {
        file << (pointIndices[index] + 1) << '/'
             << (texcoordIndices[index] + 1) << '/'
             << (normalIndices[index] + 1) << ' ';
    }

    void writeFaceNoNormals(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &texcoordIndices,
        const std::vector<int64_t> &,
        int64_t index)
    {
        file << (pointIndices[index] + 1) << '/'
             << (texcoordIndices[index] + 1) << ' ';
    }

    void writeFaceNoTexcoords(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &,
        const std::vector<int64_t> &normalIndices,
        int64_t index)
    {
        file << (pointIndices[index] + 1) << "//"
             << (normalIndices[index] + 1) << ' ';
    }

    void writeFaceOnlyPoints(
        std::ofstream &file,
        const std::vector<int64_t> &pointIndices,
        const std::vector<int64_t> &,
        const std::vector<int64_t> &,
        int64_t index)
    {
        file << (pointIndices[index] + 1) << ' ';
    }

    WriteFaceFunction chooseWriteFaceFunction(bool hasTexcoords, bool hasNormals)
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
