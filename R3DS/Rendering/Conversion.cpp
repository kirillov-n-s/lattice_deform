#include "Conversion.h"
#include "../Model.h"

namespace R3DS::Rendering::Conversion
{
    Index makeIndex(const Model &model, size_t i)
    {
        return
        {
            model.pointIndices()[i],
            model.hasTexcoords() ? model.texcoordIndices()[i] : -1,
            model.hasNormals() ? model.normalIndices()[i] : -1
        };
    }

    Vertex vertexAtIndex(const Model &model, const Index &index)
    {
        return
        {
            model.points()[index.point],
            index.texcoord != -1 ? model.texcoords()[index.texcoord] : glm::vec3 { 0.f },
            index.normal != -1 ? model.normals()[index.normal] : glm::vec3 { 0.f }
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
        const auto& face_sizes = model.nPointsByFace();
        return std::all_of(face_sizes.begin(), face_sizes.end(),
                           [](auto x) { return x == 3; });
    }


    Model triangulate(const Model &model)
    {
        const auto& point_indices = model.pointIndices();
        const auto& texcoord_indices = model.texcoordIndices();
        const auto& normal_indices = model.normalIndices();
        const auto& face_sizes = model.nPointsByFace();

        std::vector<int64_t> new_point_indices;
        std::vector<int64_t> new_texcoord_indices;
        std::vector<int64_t> new_normal_indices;
        std::vector<int64_t> new_face_sizes;

        auto has_texcoords = model.hasTexcoords();
        auto has_normals = model.hasNormals();
        auto face_count = face_sizes.size();
        for (int64_t i = 0, global_idx = 0; i < face_count; i++)
        {
            auto face_size = face_sizes[i];
            int64_t new_face_size = 0;

            for (int64_t j = 1, first = global_idx;
                 j < face_size - 1;
                 j++, new_face_size += 3)
            {
                new_point_indices.push_back(point_indices[first]);
                new_point_indices.push_back(point_indices[first + j]);
                new_point_indices.push_back(point_indices[first + j + 1]);

                if (has_texcoords)
                {
                    new_texcoord_indices.push_back(texcoord_indices[first]);
                    new_texcoord_indices.push_back(texcoord_indices[first + j]);
                    new_texcoord_indices.push_back(texcoord_indices[first + j + 1]);
                }

                if (has_normals)
                {
                    new_normal_indices.push_back(normal_indices[first]);
                    new_normal_indices.push_back(normal_indices[first + j]);
                    new_normal_indices.push_back(normal_indices[first + j + 1]);
                }
            }

            new_face_sizes.push_back(new_face_size);
            global_idx += face_size;
        }

        return Model
        {
            model.points(),
            model.texcoords(),
            model.normals(),
            new_point_indices,
            new_texcoord_indices,
            new_normal_indices,
            new_face_sizes
        };
    }
}
