#include "ObjIO.h"
#include <sstream>
#include "ReadHelpers.h"
#include "WriteHelpers.h"
#include "../Model.h"

namespace Lattice::ObjIO {
    Lattice::Model read(const std::string &path)
    {
        Model::Points points;
        Model::Texcoords texcoords;
        Model::Normals normals;
        Model::Indices pointIndices,
                       texcoordIndices,
                       normalIndices;
        Model::FaceSizes faceSizes;

        bool hasTexcoords = true,
             hasNormals = true;

        std::ifstream file { path };
        if (!file.is_open())
            throw std::invalid_argument("File does not exist.");

        std::string header;
        for (;;) {
            file >> header;
            if (file.eof())
                break;

            if (header == "v")
                points.push_back(ReadHelpers::readPoint(file));
            else if (header == "vt")
                texcoords.push_back(ReadHelpers::readTexcoord(file));
            else if (header == "vn")
                normals.push_back(ReadHelpers::readNormal(file));
            else if (header == "f") {
                std::string face;
                std::getline(file, face);
                face.erase(face.find_last_not_of(" \n\r\t") + 1);
                std::istringstream buffer { face };

                int64_t pointIdx,
                        texcoordIdx,
                        normalIdx;
                size_t faceSize = 0;
                int delim;

                for (;;) {
                    buffer >> pointIdx;
                    if (!buffer)
                        throw std::runtime_error("Bad face format.");
                    pointIndices.push_back(pointIdx - 1);
                    faceSize++;
                    if (buffer.eof()) {
                        if (hasTexcoords || hasNormals)
                            throw std::runtime_error("Inconsistent face format.");
                        break;
                    }

                    delim = buffer.get();
                    if (delim == ' ') {
                        if (hasTexcoords || hasNormals) {
                            if (!texcoordIndices.empty() || !normalIndices.empty())
                                throw std::runtime_error("Inconsistent face format.");
                            hasTexcoords = hasNormals = false;
                        }
                        continue;
                    }
                    if (delim != '/')
                        throw std::runtime_error("Bad face format.");

                    buffer >> texcoordIdx;
                    if (hasTexcoords && texcoordIndices.empty())
                        hasTexcoords = (bool)buffer;
                    else if (hasTexcoords != (bool)buffer)
                        throw std::runtime_error("Inconsistent face format.");
                    if (hasTexcoords)
                        texcoordIndices.push_back(texcoordIdx - 1);
                    if (buffer.eof()) {
                        if (hasNormals)
                            throw std::runtime_error("Inconsistent face format.");
                        break;
                    }
                    if (!buffer)
                        buffer.clear();

                    delim = buffer.get();
                    if (delim == ' ') {
                        if (hasNormals) {
                            if (!normalIndices.empty())
                                throw std::runtime_error("Inconsistent face format.");
                            hasNormals = false;
                        }
                        continue;
                    }
                    if (delim != '/')
                        throw std::runtime_error("Bad face format.");

                    buffer >> normalIdx;
                    if (!buffer)
                        throw std::runtime_error("Bad face format.");
                    if (hasNormals)
                        normalIndices.push_back(normalIdx - 1);
                    if (buffer.eof())
                        break;
                }

                faceSizes.push_back(faceSize);

                if (faceSizes.back() < 3)
                    throw std::runtime_error("Not enough face elements.");
            }
            else
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        file.close();

        return {
            points,
            texcoords,
            normals,
            pointIndices,
            texcoordIndices,
            normalIndices,
            faceSizes
        };
    }

    void write(
        const Lattice::Model &model,
        const std::string &path)
    {
        std::ofstream file { path };

        for (const auto& point : model.points())
            file << "v "
                 << point.x << ' '
                 << point.y << ' '
                 << point.z << ' '
                 << point.w << '\n';
        for (const auto& texcoord : model.texcoords())
            file << "vt "
                 << texcoord.x << ' '
                 << texcoord.y << ' '
                 << texcoord.z << '\n';
        for (const auto& normal : model.normals())
            file << "vn "
                 << normal.x << ' '
                 << normal.y << ' '
                 << normal.z << '\n';

        const auto& pointIndices = model.pointIndices();
        const auto& texcoordIndices = model.texcoordIndices();
        const auto& normalIndices = model.normalIndices();
        const auto& faceSizes = model.faceSizes();

        auto* writeFace = WriteHelpers::chooseWriteFaceFunction(
            model.hasTexcoords(),
            model.hasNormals());

        const size_t nFaces = faceSizes.size();
        size_t globalIdx = 0;
        for (size_t i = 0; i < nFaces; i++) {
            file << "f ";
            for (size_t j = 0; j < faceSizes[i]; j++, globalIdx++)
                writeFace(
                    file,
                    pointIndices,
                    texcoordIndices,
                    normalIndices,
                    globalIdx);
            file << '\n';
        }
    }
}

