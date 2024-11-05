// src/ShardedVectorDatabase.cpp
#include "ShardedVectorDatabase.hpp"

ShardedVectorDatabase::ShardedVectorDatabase(size_t num_shards_, size_t dimension_)
    : num_shards(num_shards_), dimension(dimension_) {
    for(size_t i = 0; i < num_shards; ++i){
        shards.emplace_back(std::make_unique<VectorDatabase>(dimension));
    }
}

void ShardedVectorDatabase::insert(int id, const Vector& vec) {
    size_t shard_index = hasher(id) % num_shards;
    shards[shard_index]->insert(id, vec);
}

std::vector<std::pair<int, float>> ShardedVectorDatabase::query(const Vector& vec, int k) {
    std::vector<std::pair<int, float>> all_results;
    for(auto& shard : shards){
        auto shard_results = shard->query(vec, k);
        all_results.insert(all_results.end(), shard_results.begin(), shard_results.end());
    }

    // Sort all_results based on distance and pick top k
    std::sort(all_results.begin(), all_results.end(), 
        [](const std::pair<int, float>& a, const std::pair<int, float>& b) -> bool {
            return a.second < b.second;
        });

    if(all_results.size() > static_cast<size_t>(k)){
        all_results.resize(k);
    }

    return all_results;
}
