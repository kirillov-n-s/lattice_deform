#ifndef LATTICE_DEFORM_SPATIALVIEWS_UNIFORMGRID_H
#define LATTICE_DEFORM_SPATIALVIEWS_UNIFORMGRID_H

#include <vector>
#include "../BoundingBox.h"
#include "../../Model.h"

namespace Lattice::Deform::SpatialViews {
    class UniformGrid
    {
    public:
        using PointIterator = Model::Points::const_iterator;

        UniformGrid(
            const PointIterator &first,
            const PointIterator &last,
            const BoundingBox &boundingBox,
            const float cellSize);

        template<typename Function>
        void traverse(
            const BoundingBox &boundingBox,
            Function function) const;

    private:
        using Grid = std::vector<std::vector<PointIterator>>;
        using CellIterator = Grid::const_iterator;

        Grid m_grid;
        glm::uvec3 m_extents;

        BoundingBox m_boundingBox;
        glm::vec3 m_offset;
        float m_cellSize;

        glm::uvec3 index(const glm::vec3 &point) const;
        size_t hash(const glm::uvec3 &index) const;

        void insert(const PointIterator &pointIt);
        std::vector<CellIterator> query(const BoundingBox&) const;
    };
}









//Implementation
namespace Lattice::Deform::SpatialViews {
    template<typename Function>
    void UniformGrid::traverse(
        const BoundingBox &boundingBox,
        Function function) const
    {
        const auto& cellIts = query(boundingBox);
        for (const auto& cellIt : cellIts)
            for (const auto& pointIt : *cellIt)
                function(pointIt);
    }
}

#endif
