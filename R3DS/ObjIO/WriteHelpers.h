#pragma once
#include <vector>
#include <fstream>

namespace R3DS::ObjIO::WriteHelpers
{
    void writeFaceFull(
        std::ofstream&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        int64_t);
    void writeFaceNoNormals(
        std::ofstream&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        int64_t);
    void writeFaceNoTexcoords(
        std::ofstream&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        int64_t);
    void writeFaceOnlyPoints(
        std::ofstream&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        int64_t);

    using WriteFaceFunction = void (*) (
        std::ofstream&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        const std::vector<int64_t>&,
        int64_t);
    WriteFaceFunction chooseWriteFaceFunction(bool, bool);
}
