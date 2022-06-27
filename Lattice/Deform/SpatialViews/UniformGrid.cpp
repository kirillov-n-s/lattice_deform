#include <iterator>
#include "UniformGrid.h"

namespace Lattice::Deform::SpatialViews {
    glm::uvec3 UniformGrid::index(const glm::vec3 &point) const
    {
        return glm::clamp(
            glm::uvec3 { glm::floor((point + m_offset) / m_cellSize) },
            glm::uvec3 { 0 },
            m_extents - 1u);
    }

    size_t UniformGrid::hash(const glm::uvec3 &index) const
    {
        return index.x + m_extents.x * (index.y + m_extents.y * index.z);
    }

    void UniformGrid::insert(const PointIterator &pointIt)
    {
        const auto& point = *pointIt;
        if (!m_boundingBox.contains(point))
            return;
        m_grid[hash(index(point))].push_back(pointIt);
    }

    std::vector<UniformGrid::CellIterator> UniformGrid::query(const BoundingBox &boundingBox) const
    {
        std::vector<CellIterator> result;
        if (!m_boundingBox.overlaps(boundingBox))
            return result;
        const auto& cellBegin = m_grid.begin();
        const auto& minIndex = index(boundingBox.min);
        const auto& maxIndex = index(boundingBox.max);
        for (auto x = minIndex.x; x <= maxIndex.x; x++)
            for (auto y = minIndex.y; y <= maxIndex.y; y++)
                for (auto z = minIndex.z; z <= maxIndex.z; z++) {
                    const auto& cellIt = cellBegin + hash({ x, y, z });
                    if (!cellIt->empty())
                        result.push_back(cellIt);
                }
        return result;
    }

    UniformGrid::UniformGrid(
        const PointIterator &first,
        const PointIterator &last,
        const BoundingBox &boundingBox,
        const float cellSize)
        : m_extents(glm::ceil(boundingBox.diag() / cellSize)),
          m_boundingBox(boundingBox),
          m_offset(-boundingBox.min),
          m_cellSize(cellSize)
    {
        m_grid.resize(m_extents.x * m_extents.y * m_extents.z);
        for (auto pointIt = first; pointIt != last; pointIt++)
            insert(pointIt);
    }
}



