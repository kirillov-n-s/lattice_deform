#include "Conversion.h"
#include "../Model.h"
#include <algorithm>

namespace Lattice::Rendering::Conversion {
    Index makeIndex(
        const Model &model,
        const size_t flatIndex)
    {
        return {
            model.pointIndices()[flatIndex],
            model.hasTexcoords()
                ? model.texcoordIndices()[flatIndex]
                : -1,
            model.hasNormals()
                ? model.normalIndices()[flatIndex]
                : -1
        };
    }

    Vertex vertexAtIndex(
        const Model &model,
        const Index &index)
    {
        return {
            model.points()[index.point],
            index.texcoord != -1
                ? model.texcoords()[index.texcoord]
                : glm::vec3 { 0.f },
            index.normal != -1
                ? model.normals()[index.normal]
                : glm::vec3 { 0.f }
        };
    }

    bool operator==(const Index &lhs, const Index &rhs)
    {
        return lhs.point == rhs.point
            && lhs.texcoord == rhs.texcoord
            && lhs.normal == rhs.normal;
    }

    bool operator!=(const Index &lhs, const Index &rhs)
    {
        return !(lhs == rhs);
    }

    bool isTriangle(const Model &model)
    {
        const auto& faceSizes = model.faceSizes();
        return std::all_of(
            faceSizes.begin(),
            faceSizes.end(),
            [](const size_t faceSize) { return faceSize == 3; });
    }


    Model triangulate(const Model &model)
    {
        const auto& pointIndices = model.pointIndices();
        const auto& texcoordIndices = model.texcoordIndices();
        const auto& normalIndices = model.normalIndices();
        const auto& faceSizes = model.faceSizes();

        Model::Indices newPointIndices;
        Model::Indices newTexcoordIndices;
        Model::Indices newNormalIndices;
        Model::FaceSizes newFaceSizes;

        const bool hasTexcoords = model.hasTexcoords();
        const bool hasNormals = model.hasNormals();
        const size_t nFaces = faceSizes.size();
        for (size_t i = 0, globalIdx = 0; i < nFaces; i++) {
            const size_t faceSize = faceSizes[i];
            size_t newFaceSize = 0;

            for (size_t j = 1, first = globalIdx;
                 j < faceSize - 1;
                 j++, newFaceSize += 3) {
                newPointIndices.push_back(pointIndices[first]);
                newPointIndices.push_back(pointIndices[first + j]);
                newPointIndices.push_back(pointIndices[first + j + 1]);

                if (hasTexcoords) {
                    newTexcoordIndices.push_back(texcoordIndices[first]);
                    newTexcoordIndices.push_back(texcoordIndices[first + j]);
                    newTexcoordIndices.push_back(texcoordIndices[first + j + 1]);
                }

                if (hasNormals) {
                    newNormalIndices.push_back(normalIndices[first]);
                    newNormalIndices.push_back(normalIndices[first + j]);
                    newNormalIndices.push_back(normalIndices[first + j + 1]);
                }
            }

            newFaceSizes.push_back(newFaceSize);
            globalIdx += faceSize;
        }

        return {
            model.points(),
            model.texcoords(),
            model.normals(),
            newPointIndices,
            newTexcoordIndices,
            newNormalIndices,
            newFaceSizes
        };
    }

    std::pair<std::vector<Vertex>, std::vector<unsigned int>> indexedVertices(const Model &model)
    {
        const auto& triangleModel = isTriangle(model)
                                    ? model
                                    : triangulate(model);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> flatIndices;
        std::unordered_map<Index, unsigned int> indexMap;

        const auto nPointIndices = triangleModel.pointIndices().size();
        for (size_t pointIdx = 0; pointIdx < nPointIndices; pointIdx++) {
            const auto& convIdx = makeIndex(triangleModel, pointIdx);
            const auto& indexPairIt = indexMap.find(convIdx);
            if (indexPairIt != indexMap.end()) {
                flatIndices.push_back(indexPairIt->second);
                continue;
            }
            const size_t flatIdx = indexMap.size();
            indexMap[convIdx] = flatIdx;
            flatIndices.push_back(flatIdx);
            vertices.push_back(vertexAtIndex(triangleModel, convIdx));
        }

        return { vertices, flatIndices };
    }
}
