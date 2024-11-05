// tests/TestVectorDatabase.cpp
#include "../include/ShardedVectorDatabase.hpp"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    size_t num_shards = 2;
    size_t vector_dim = 128;
    ShardedVectorDatabase db(num_shards, vector_dim);

    // Insert vectors
    std::vector<float> vec1(vector_dim, 0.1f);
    std::vector<float> vec2(vector_dim, 0.2f);
    std::vector<float> vec3(vector_dim, 0.3f);

    db.insert(1, vec1);
    db.insert(2, vec2);
    db.insert(3, vec3);

    // Query vectors
    auto results = db.query(vec1, 2);
    assert(results.size() == 2);
    assert(results[0].first == 1); // Nearest to itself

    // The second nearest might vary based on KD-tree implementation and sharding
    assert(results[1].first == 2 || results[1].first == 3);

    std::cout << "TestVectorDatabase Passed!" << std::endl;
    return 0;
}
