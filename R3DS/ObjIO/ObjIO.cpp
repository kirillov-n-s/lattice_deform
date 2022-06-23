#include "ObjIO.h"
#include "ReadHelpers.h"
#include "WriteHelpers.h"
#include "../Model.h"

namespace R3DS::ObjIO
{
    R3DS::Model read(const std::string &path)
    {
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec3> texcoords, normals;
        std::vector<int64_t> vertexIndices, texcoordIndices, normalIndices, nPointsByFace;

        bool hasTexcoords = true, hasNormals = true;

        std::ifstream file { path };
        if (!file.is_open())
            throw std::invalid_argument("File does not exist.");

        std::string header;
        for (;;)
        {
            file >> header;
            if (file.eof())
                break;

            if (header == "v")
                vertices.push_back(ReadHelpers::readPoint(file));
            else if (header == "vt")
                texcoords.push_back(ReadHelpers::readTexcoord(file));
            else if (header == "vn")
                normals.push_back(ReadHelpers::readNormal(file));
            else if (header == "f")
            {
                std::string face;
                std::getline(file, face);
                face.erase(face.find_last_not_of(" \n\r\t") + 1);
                std::istringstream buffer { face };

                int64_t iv, ivt, ivn, face_size = 0;
                int delim;

                for (;;)
                {
                    buffer >> iv;
                    if (!buffer)
                        throw std::runtime_error("Bad face format.");
                    vertexIndices.push_back(iv - 1);
                    face_size++;
                    if (buffer.eof())
                    {
                        if (hasTexcoords || hasNormals)
                            throw std::runtime_error("Inconsistent face format.");
                        break;
                    }

                    delim = buffer.get();
                    if (delim == ' ')
                    {
                        if (hasTexcoords || hasNormals)
                        {
                            if (!texcoordIndices.empty() || !normalIndices.empty())
                                throw std::runtime_error("Inconsistent face format.");
                            hasTexcoords = hasNormals = false;
                        }
                        continue;
                    }
                    if (delim != '/')
                        throw std::runtime_error("Bad face format.");

                    buffer >> ivt;
                    if (hasTexcoords && texcoordIndices.empty())
                        hasTexcoords = (bool)buffer;
                    else if (hasTexcoords != (bool)buffer)
                        throw std::runtime_error("Inconsistent face format.");
                    if (hasTexcoords)
                        texcoordIndices.push_back(ivt - 1);
                    if (buffer.eof())
                    {
                        if (hasNormals)
                            throw std::runtime_error("Inconsistent face format.");
                        break;
                    }
                    if (!buffer)
                        buffer.clear();

                    delim = buffer.get();
                    if (delim == ' ')
                    {
                        if (hasNormals)
                        {
                            if (!normalIndices.empty())
                                throw std::runtime_error("Inconsistent face format.");
                            hasNormals = false;
                        }
                        continue;
                    }
                    if (delim != '/')
                        throw std::runtime_error("Bad face format.");

                    buffer >> ivn;
                    if (!buffer)
                        throw std::runtime_error("Bad face format.");
                    if (hasNormals)
                        normalIndices.push_back(ivn - 1);
                    if (buffer.eof())
                        break;
                }

                nPointsByFace.push_back(face_size);

                if (nPointsByFace.back() < 3)
                    throw std::runtime_error("Not enough face elements.");
            }
            else
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        file.close();

        return
        {
                vertices,
                texcoords,
                normals,
                vertexIndices,
                texcoordIndices,
                normalIndices,
                nPointsByFace
        };
    }

    void write(const R3DS::Model &model, const std::string &path)
    {
        std::ofstream file { path };

        for (const auto& point : model.points())
            file << "v " << point.x << ' ' << point.y << ' ' << point.z << ' ' << point.w << '\n';
        for (const auto& texcoord : model.texcoords())
            file << "vt " << texcoord.x << ' ' << texcoord.y << ' ' << texcoord.z << '\n';
        for (const auto& normal : model.normals())
            file << "vn " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';

        const auto& point_indices = model.pointIndices();
        const auto& texcoord_indices = model.texcoordIndices();
        const auto& normal_indices = model.normalIndices();
        const auto& face_sizes = model.nPointsByFace();

        auto* writeFace = WriteHelpers::chooseWriteFaceFunction(model.hasTexcoords(), model.hasNormals());

        auto face_count = face_sizes.size();
        int64_t global_idx = 0;
        for (int64_t i = 0; i < face_count; i++)
        {
            file << "f ";
            for (int64_t j = 0; j < face_sizes[i]; j++, global_idx++)
                writeFace(file, point_indices, texcoord_indices, normal_indices, global_idx);
            file << '\n';
        }
    }
}

