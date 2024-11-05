// include/KDTree.hpp
#ifndef KDTREE_HPP
#define KDTREE_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include <queue>
#include <cmath>
#include "Vector.hpp"

struct KDNode {
    Vector point;
    int id;
    int axis;
    std::unique_ptr<KDNode> left;
    std::unique_ptr<KDNode> right;

    KDNode(const Vector& pt, int identifier, int ax)
        : point(pt), id(identifier), axis(ax), left(nullptr), right(nullptr) {}
};

class KDTree {
private:
    std::unique_ptr<KDNode> root;
    size_t dimension;
    std::vector<std::pair<Vector, int>> points;

    std::unique_ptr<KDNode> buildTree(int depth, int start, int end);
    void nearestNeighborSearch(const KDNode* node, const Vector& target, int k,
                               std::priority_queue<std::pair<float, int>>& max_heap) const;

public:
    KDTree(size_t dim);
    ~KDTree();

    void insert(int id, const Vector& vec);
    std::vector<std::pair<int, float>> query(const Vector& vec, int k) const;
};

#endif // KDTREE_HPP
