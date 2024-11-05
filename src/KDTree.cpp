// src/KDTree.cpp
#include "KDTree.hpp"
#include <queue>

// Constructor
KDTree::KDTree(size_t dim)
    : root(nullptr), dimension(dim) {}

// Destructor
KDTree::~KDTree() {}

// Helper function to build the KD-tree recursively
std::unique_ptr<KDNode> KDTree::buildTree(int depth, int start, int end) {
    if (start >= end) return nullptr;

    int axis = depth % dimension;
    int mid = start + (end - start) / 2;

    std::nth_element(points.begin() + start, points.begin() + mid, points.begin() + end,
        [axis](const std::pair<Vector, int>& a, const std::pair<Vector, int>& b) -> bool {
            return a.first.data[axis] < b.first.data[axis];
        });

    auto node = std::make_unique<KDNode>(points[mid].first, points[mid].second, axis);
    node->left = buildTree(depth + 1, start, mid);
    node->right = buildTree(depth + 1, mid + 1, end);

    return node;
}

// Insert function (for simplicity, adding to points and rebuilding the tree)
void KDTree::insert(int id, const Vector& vec) {
    points.emplace_back(vec, id);
    root = buildTree(0, 0, points.size());
}

// Query function using nearest neighbor search
std::vector<std::pair<int, float>> KDTree::query(const Vector& vec, int k) const {
    std::priority_queue<std::pair<float, int>> max_heap; // max heap to store top k closest points

    nearestNeighborSearch(root.get(), vec, k, max_heap);

    std::vector<std::pair<int, float>> results;
    while (!max_heap.empty()) {
        results.emplace_back(std::make_pair(max_heap.top().second, max_heap.top().first));
        max_heap.pop();
    }

    // Reverse to get nearest first
    std::reverse(results.begin(), results.end());
    return results;
}

// Recursive nearest neighbor search
void KDTree::nearestNeighborSearch(const KDNode* node, const Vector& target, int k,
                                   std::priority_queue<std::pair<float, int>>& max_heap) const {
    if (!node) return;

    float distance = target.euclidean_distance(node->point);
    if (static_cast<size_t>(max_heap.size()) < static_cast<size_t>(k)) {
        max_heap.emplace(distance, node->id);
    } else if (distance < max_heap.top().first) {
        max_heap.pop();
        max_heap.emplace(distance, node->id);
    }

    int axis = node->axis;
    float diff = target.data[axis] - node->point.data[axis];

    const KDNode* first = diff < 0 ? node->left.get() : node->right.get();
    const KDNode* second = diff < 0 ? node->right.get() : node->left.get();

    nearestNeighborSearch(first, target, k, max_heap);

    if (static_cast<size_t>(max_heap.size()) < static_cast<size_t>(k) || std::abs(diff) < max_heap.top().first) {
        nearestNeighborSearch(second, target, k, max_heap);
    }
}
