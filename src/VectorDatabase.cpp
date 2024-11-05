// src/VectorDatabase.cpp
#include "VectorDatabase.hpp"
#include "VectorDBException.hpp"

VectorDatabase::VectorDatabase(size_t dim)
    : dimension(dim) {
    kd_tree = new KDTree(dim);
}

VectorDatabase::~VectorDatabase() {
    delete kd_tree;
}

void VectorDatabase::insert(int id, const Vector& vec) {
    if (vec.data.size() != dimension) {
        throw VectorDBException("Vector dimension mismatch", 400);
    }

    std::unique_lock<std::shared_mutex> lock(db_mutex);
    kd_tree->insert(id, vec);
    // Optionally, maintain id_map if needed
}

std::vector<std::pair<int, float>> VectorDatabase::query(const Vector& vec, int k) const {
    if (vec.data.size() != dimension) {
        throw VectorDBException("Vector dimension mismatch", 400);
    }

    std::shared_lock<std::shared_mutex> lock(db_mutex);
    return kd_tree->query(vec, k);
}
