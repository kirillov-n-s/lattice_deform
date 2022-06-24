#pragma once
#include <vector>
#include "../../Model.h"

namespace Internship
{
    struct BoundingBox;
}

namespace Internship::Deform::SpatialViews
{
    class KDTree3D
    {
    private:
        using PointIterator = Model::PointIterator;
        using ItemIterator = std::vector<PointIterator>::iterator;

        struct Node
        {
            virtual ~Node() = default;
        };

        struct Branch : Node
        {
            float split;
            Node* left  = nullptr;
            Node* right = nullptr;

            Branch(float);
            ~Branch();
        };

        struct Leaf : Node
        {
            ItemIterator first, last;

            Leaf(const ItemIterator&, const ItemIterator&);
        };

    private:
        std::vector<PointIterator> _items;
        Node* _root;
        size_t _leaf_size;

        Node* make_subtree(ItemIterator, ItemIterator, int);
        void query_subtree(const Node*, const BoundingBox&, std::vector<const Leaf*>&, int) const;
        std::vector<const Leaf*> query(const BoundingBox&) const;

    public:
        KDTree3D(const PointIterator&, const PointIterator&, const BoundingBox&, size_t);
        ~KDTree3D();

        template<typename Function>
        void traverse(const BoundingBox&, Function) const;
    };
}

namespace Internship::Deform::SpatialViews
{
    template<typename Function>
    void KDTree3D::traverse(const BoundingBox &boundingBox, Function function) const
    {
        const auto& leafPtrs = query(boundingBox);
        for (const auto& leafPtr : leafPtrs)
            for (auto itemIt = leafPtr->first; itemIt != leafPtr->last; itemIt++)
                function(*itemIt);
    }
}
