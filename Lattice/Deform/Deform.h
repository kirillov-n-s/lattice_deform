#ifndef LATTICE_DEFORM_DEFORM_H
#define LATTICE_DEFORM_DEFORM_H

#include <stdexcept>
#include <glm/gtx/norm.hpp>
#include "SpatialViews/UniformGrid.h"
#include "SpatialViews/KDTree3D.h"
#include "../Model.h"

namespace Lattice::Deform {
    template<
        typename SpatialView,
        typename Kernel,
        typename... SpatialViewArgs>
    Model lattice(
        const Model &restPointIt,
        const Model &rest,
        const Model &deformed,
        const Kernel &kernel,
        const float radius,
        SpatialViewArgs&&... spatialViewArgs);
}









//Implementation
namespace Lattice::Deform {
    template<
        typename SpatialView,
        typename Kernel,
        typename... SpatialViewArgs>
    Model lattice(
        const Model &toDeform,
        const Model &rest,
        const Model &deformed,
        const Kernel &kernel,
        const float radius,
        SpatialViewArgs&&... spatialViewArgs)
    {
        const auto& restPoints = rest.points();
        const auto& restIndices = rest.pointIndices();
        const auto& deformedPoints = deformed.points();
        const auto& deformedIndices = deformed.pointIndices();

        if (restIndices != deformedIndices)
            throw std::runtime_error("Rest and deformed geometry mismatch.");

        const auto& toDeformPoints = toDeform.points();
        auto resultPoints = toDeformPoints;

        const BoundingBox& boundingBox = makeBoundingBox(toDeform);

        SpatialView spatialView {
            restPoints.begin(),
            restPoints.end(),
            { boundingBox.min - radius, boundingBox.max + radius },
            std::forward<SpatialViewArgs>(spatialViewArgs)...
        };

        const auto& restPointsBegin = restPoints.begin();
        const auto& deformedPointsBegin = deformedPoints.begin();
        const auto& toDeformPointsBegin = toDeformPoints.begin();
        const auto& toDeformPointsEnd = toDeformPoints.end();
        const auto& resultPointsBegin = resultPoints.begin();
        for (auto toDeformPointIt = toDeformPointsBegin; toDeformPointIt != toDeformPointsEnd; toDeformPointIt++) {
            glm::vec3 offset { 0.f };
            float weights = 0.f;

            spatialView.traverse(
                {
                    glm::vec3{*toDeformPointIt - radius},
                    glm::vec3{*toDeformPointIt + radius}
                },
                [&toDeformPointIt,
                 &deformedPointsBegin,
                 &restPointsBegin,
                 &offset,
                 &radius,
                 &kernel,
                 &weights]
                (const auto& restPointIt)
                {
                    const float distance = glm::distance(*toDeformPointIt, *restPointIt);
                    if (distance > radius)
                        return;
                    const float weight = kernel(distance, radius);
                    weights += weight;
                    offset += weight * glm::vec3 {
                        deformedPointsBegin[restPointIt - restPointsBegin] - *restPointIt
                    };
                }
            );

            if (weights != 0.f)
                resultPointsBegin[toDeformPointIt - toDeformPointsBegin]
                += glm::vec4 { offset / weights, 0.f };
        }

        return Model {
            resultPoints,
            toDeform.texcoords(),
            {},
            toDeform.pointIndices(),
            toDeform.texcoordIndices(),
            {},
            toDeform.faceSizes()
        };
    }
}

#endif
