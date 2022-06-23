#include <algorithm>
#include <typeinfo>
#include "KDTree3D.h"
#include "../BoundingBox.h"

namespace R3DS::Deform::SpatialViews
{
    KDTree3D::Branch::Branch(float split)
            : split(split)
    {}

    KDTree3D::Branch::~Branch()
    {
        delete left;
        delete right;
    }

    KDTree3D::Leaf::Leaf(const ItemIterator &first, const ItemIterator &last)
            : first(first), last(last)
    {}

    KDTree3D::Node* KDTree3D::make_subtree(ItemIterator first, ItemIterator last, int dim)
    {
        auto dist = last - first;
        if (dist <= _leaf_size)
            return new Leaf {first, last };

        auto median = first + dist / 2;
        std::nth_element(first, median, last,
                         [&dim](const auto& lhs, const auto& rhs)
                         {
                             return (*lhs)[dim] < (*rhs)[dim];
                         });
        auto split = (**median)[dim];
        auto branch = new Branch {split };
        dim = (dim + 1) % 3;
        branch->left = make_subtree(first, median, dim);
        branch->right = make_subtree(median, last, dim);

        return branch;
    }

    void KDTree3D::query_subtree(const Node *node, const BoundingBox &bbox,
                                 std::vector<const Leaf*> &result, int dim) const
    {
        if (typeid(*node) == typeid(Leaf))
            return result.push_back(static_cast<const Leaf*>(node));

        auto branch = static_cast<const Branch*>(node);
        auto new_dim = (dim + 1) % 3;
        if (bbox.min[dim] < branch->split)
            query_subtree(branch->left, bbox, result, new_dim);
        if (bbox.max[dim] > branch->split)
            query_subtree(branch->right, bbox, result, new_dim);
    }

    KDTree3D::KDTree3D(const PointIterator &first, const PointIterator &last,
                       const BoundingBox &bbox, size_t leaf_size)
            : _leaf_size(leaf_size)
    {
        for (auto it = first; it != last; it++)
            if (bbox.contains(*it))
                _items.push_back(it);
        _root = make_subtree(_items.begin(), _items.end(), 0);
    }

    KDTree3D::~KDTree3D()
    {
        delete _root;
    }

    std::vector<const KDTree3D::Leaf*> KDTree3D::query(const BoundingBox &bbox) const
    {
        std::vector<const KDTree3D::Leaf*> result;
        query_subtree(_root, bbox, result, 0);
        return result;
    }
}

