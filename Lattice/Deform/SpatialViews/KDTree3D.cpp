#include <algorithm>
#include <typeinfo>
#include "KDTree3D.h"
#include "../BoundingBox.h"

namespace Lattice::Deform::SpatialViews {
    KDTree3D::Branch::Branch(float splitValue)
        : splitValue(splitValue)
    {}

    KDTree3D::Branch::~Branch()
    {
        delete left;
        delete right;
    }

    KDTree3D::Leaf::Leaf(
        const PointItersIterator &first,
        const PointItersIterator &last)
        : first(first), last(last)
    {}

    KDTree3D::Node* KDTree3D::makeSubtree(
        const PointItersIterator &first,
        const PointItersIterator &last,
        const uint8_t dim)
    {
        const auto& dist = last - first;
        if (dist <= m_leafSize)
            return new Leaf { first, last };

        const auto& median = first + dist / 2;
        std::nth_element(
            first,
            median,
            last,
            [&dim](const auto& lhs, const auto& rhs)
            {
                return (*lhs)[dim] < (*rhs)[dim];
            });
        auto* branch = new Branch { (**median)[dim] };
        const uint8_t newDim = (dim + 1) % 3;
        branch->left = makeSubtree(first, median, newDim);
        branch->right = makeSubtree(median, last, newDim);

        return branch;
    }

    void KDTree3D::querySubtree(
        const Node *node,
        const BoundingBox &boundingBox,
        const uint8_t dim,
        std::vector<const Leaf*> &result) const
    {
        if (typeid(*node) == typeid(Leaf))
            return result.push_back(static_cast<const Leaf*>(node));

        const auto* branch = static_cast<const Branch*>(node);
        const uint8_t newDim = (dim + 1) % 3;
        if (boundingBox.min[dim] < branch->splitValue)
            querySubtree(
                branch->left,
                boundingBox,
                newDim,
                result);
        if (boundingBox.max[dim] > branch->splitValue)
            querySubtree(
                branch->right,
                boundingBox,
                newDim,
                result);
    }

    std::vector<const KDTree3D::Leaf*> KDTree3D::query(const BoundingBox &boundingBox) const
    {
        std::vector<const KDTree3D::Leaf*> result;
        querySubtree(
            m_root,
            boundingBox,
            0,
            result);
        return result;
    }

    KDTree3D::KDTree3D(
        const PointIterator &first,
        const PointIterator &last,
        const BoundingBox &boundingBox,
        const size_t leafSize)
        : m_leafSize(leafSize)
    {
        for (auto pointIt = first; pointIt != last; pointIt++)
            if (boundingBox.contains(*pointIt))
                m_items.push_back(pointIt);
        m_root = makeSubtree(
            m_items.begin(),
            m_items.end(),
            0);
    }

    KDTree3D::~KDTree3D()
    {
        delete m_root;
    }
}

