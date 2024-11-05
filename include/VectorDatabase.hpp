// include/VectorDatabase.hpp
#ifndef VECTORDATABASE_HPP
#define VECTORDATABASE_HPP

#include "Vector.hpp"
#include "KDTree.hpp"
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <utility>
#include <memory>

class VectorDatabase {
private:
    KDTree* kd_tree;
    size_t dimension;
    std::unordered_map<int, int> id_map; // Map from ID to index in KD-tree (if needed)
    std::shared_mutex db_mutex;

public:
    VectorDatabase(size_t dim);
    ~VectorDatabase();

    void insert(int id, const Vector& vec);
    std::vector<std::pair<int, float>> query(const Vector& vec, int k) const;
};

#endif // VECTORDATABASE_HPP
