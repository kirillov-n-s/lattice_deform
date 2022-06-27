#ifndef LATTICE_DEFORM_SPATIALVIEWS_KDTREE3D_H
#define LATTICE_DEFORM_SPATIALVIEWS_KDTREE3D_H

#include <vector>
#include "../../Model.h"

namespace Lattice {
    struct BoundingBox;
}

namespace Lattice::Deform::SpatialViews {
    class KDTree3D
    {
    public:
        using PointIterator = Model::Points::const_iterator;

        KDTree3D(
            const PointIterator &first,
            const PointIterator &last,
            const BoundingBox &boundingBox,
            const size_t leafSize);
        ~KDTree3D();

        template<typename Function>
        void traverse(
            const BoundingBox &boundingBox,
            Function function) const;

    private:
        using PointItersIterator = std::vector<PointIterator>::iterator;

        struct Node
        {
            virtual ~Node() = default;
        };

        struct Branch : Node
        {
            float splitValue;
            Node* left  = nullptr;
            Node* right = nullptr;

            Branch(float splitValue);
            ~Branch();
        };

        struct Leaf : Node
        {
            PointItersIterator first;
            PointItersIterator last;
            Leaf(const PointItersIterator &first, const PointItersIterator &last);
        };

        std::vector<PointIterator> m_items;
        Node* m_root;
        size_t m_leafSize;

        Node* makeSubtree(
            const PointItersIterator &first,
            const PointItersIterator &last,
            const uint8_t dim);
        void querySubtree(
            const Node *node,
            const BoundingBox &boundingBox,
            const uint8_t dim,
            std::vector<const Leaf*> &result) const;
        std::vector<const Leaf*> query(const BoundingBox &boundingBox) const;
    };
}









//Implementation
namespace Lattice::Deform::SpatialViews {
    template<typename Function>
    void KDTree3D::traverse(
        const BoundingBox &boundingBox,
        Function function) const
    {
        const auto& leafPtrs = query(boundingBox);
        for (auto* leafPtr : leafPtrs)
            for (auto pointItIt = leafPtr->first; pointItIt != leafPtr->last; pointItIt++)
                function(*pointItIt);
    }
}

#endif
