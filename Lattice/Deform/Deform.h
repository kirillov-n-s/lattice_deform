#pragma once
#include <stdexcept>
#include <glm/gtx/norm.hpp>
#include "SpatialViews/UniformGrid.h"
#include "SpatialViews/KDTree3D.h"
#include "../Model.h"

namespace Lattice::Deform
{
    template<typename SpatialView, typename Kernel, typename... Args>
    Model lattice(const Model&, const Model&, const Model&, const Kernel&, float, Args&&...);
}

namespace Lattice::Deform
{
    template<typename SpatialView, typename Kernel, typename... Args>
    Model lattice(const Model &source, const Model &rest, const Model &deformed,
                  const Kernel &kernel, float radius, Args&&... args)
    {
        const auto& rest_points = rest.points();
        const auto& rest_indices = rest.pointIndices();
        const auto& deformed_points = deformed.points();
        const auto& deformed_indices = deformed.pointIndices();

        auto rest_point_count = rest_points.size();
        auto rest_index_count = rest_indices.size();
        auto deformed_point_count = deformed_points.size();
        auto deformed_index_count = deformed_indices.size();

        if (rest_point_count != deformed_point_count || rest_index_count != deformed_index_count)
            throw std::runtime_error("Rest and deformed geometry mismatch.");

        const auto& source_points = source.points();
        auto result_points = source_points;

        const auto& bbox = makeBoundingBox(source);
        SpatialView spatialView
        {
            rest_points.begin(), rest_points.end(),
            { bbox.min - radius, bbox.max + radius },
            std::forward<Args>(args)...
        };

        const auto& rest_begin = rest_points.begin();
        const auto& deformed_begin = deformed_points.begin();
        const auto& source_begin = source_points.begin();
        const auto& source_end = source_points.end();
        const auto& result_begin = result_points.begin();
        for (auto sourceIt = source_begin; sourceIt != source_end; sourceIt++)
        {
            glm::vec3 offset { 0.f };
            float weights = 0.f, weight;

            spatialView.traverse(
                {
                    glm::vec3{*sourceIt - radius},
                    glm::vec3{*sourceIt + radius}
                },
                [&sourceIt, &deformed_begin, &rest_begin, &offset, &radius, &weight, &kernel, &weights]
                (const auto& restIt)
                {
                    auto distance = glm::distance(*sourceIt, *restIt);
                    offset +=
                            glm::vec3 { (*(deformed_begin + (restIt - rest_begin)) - *restIt) }
                            * (distance <= radius
                               ? (weight = kernel(distance, radius), weights += weight, weight)
                               : 0.f);
                }
            );

            if (weights != 0.f)
                *(result_begin + (sourceIt - source_begin)) += glm::vec4 { offset / weights, 0.f };
        }

        return Model
        {
            result_points,
            source.texcoords(),
            {},
            source.pointIndices(),
            source.texcoordIndices(),
            {},
            source.nPointsByFace()
        };
    }
}
