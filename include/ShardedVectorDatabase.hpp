// include/ShardedVectorDatabase.hpp
#ifndef SHARDEDVECTORDATABASE_HPP
#define SHARDEDVECTORDATABASE_HPP

#include "VectorDatabase.hpp"
#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>

class ShardedVectorDatabase {
private:
    std::vector<std::unique_ptr<VectorDatabase>> shards;
    size_t num_shards;
    size_t dimension;
    std::hash<int> hasher;

public:
    ShardedVectorDatabase(size_t num_shards_, size_t dimension_);
    void insert(int id, const Vector& vec);
    std::vector<std::pair<int, float>> query(const Vector& vec, int k);
};

#endif // SHARDEDVECTORDATABASE_HPP
