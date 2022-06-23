#include <iterator>
#include "UniformGrid.h"

namespace R3DS::Deform::SpatialViews
{
    glm::uvec3 UniformGrid::index(const glm::vec3 &point) const
    {
        return glm::clamp(glm::uvec3 { glm::floor((point + _offset) / _cell_size) },
                          glm::uvec3 { 0 },
                          _extents - 1u);
    }

    size_t UniformGrid::hash(const glm::uvec3 &index) const
    {
        return index.x + _extents.x * (index.y + _extents.y * index.z);
    }

    void UniformGrid::insert(const PointIterator &value)
    {
        auto point = *value;
        if (!_bbox.contains(point))
            return;
        _grid[hash(index(point))].push_back(value);
    }

    UniformGrid::UniformGrid(const PointIterator &first, const PointIterator &last,
                             const BoundingBox &bbox, float cell_size)
        : _extents(glm::ceil(bbox.diag() / cell_size)),
          _bbox(bbox),
          _offset(-bbox.min),
          _cell_size(cell_size)
    {
        _grid.resize(_extents.x * _extents.y * _extents.z);
        for (auto it = first; it != last; it++)
            insert(it);
    }

    std::vector<UniformGrid::CellIterator> UniformGrid::query(const BoundingBox &bbox) const
    {
        std::vector<CellIterator> result;
        if (!_bbox.overlaps(bbox))
            return result;
        auto begin = _grid.begin();
        auto min = index(bbox.min);
        auto max = index(bbox.max);
        for (int64_t x = min.x; x <= max.x; x++)
            for (int64_t y = min.y; y <= max.y; y++)
                for (int64_t z = min.z; z <= max.z; z++)
                {
                    const auto& it = begin + hash({ x, y, z });
                    if (!it->empty())
                        result.push_back(it);
                }

        return result;
    }
}



