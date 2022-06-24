#pragma once
#include <vector>
#include "../BoundingBox.h"
#include "../../Model.h"

namespace Lattice::Deform::SpatialViews
{
    class UniformGrid
    {
    private:
        using PointIterator = Model::PointIterator;
        using Grid = std::vector<std::vector<PointIterator>>;
        using CellIterator = Grid::const_iterator;

        Grid _grid;
        glm::uvec3 _extents;

        BoundingBox _bbox;
        glm::vec3 _offset;
        float _cell_size;

        glm::uvec3 index(const glm::vec3&) const;
        size_t hash(const glm::uvec3&) const;

        void insert(const PointIterator&);
        std::vector<CellIterator> query(const BoundingBox&) const;

    public:
        UniformGrid(const PointIterator&, const PointIterator&, const BoundingBox&, float);
        template<typename Function>
        void traverse(const BoundingBox&, Function);
    };
}

namespace Lattice::Deform::SpatialViews
{
    template<typename Function>
    void UniformGrid::traverse(const BoundingBox &boundingBox, Function function)
    {
        const auto& cellIts = query(boundingBox);
        for (const auto& cellIt : cellIts)
            for (const auto& itemIt : *cellIt)
                function(itemIt);
    }
}
