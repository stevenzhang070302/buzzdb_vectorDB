// tests/TestBuzzDB.cpp
#include "../include/BuzzDB.hpp"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    size_t num_shards = 2;
    size_t vector_dim = 128;
    BuzzDB db(num_shards, vector_dim);

    // Insert vectors
    std::vector<float> vec1(vector_dim, 0.1f);
    std::vector<float> vec2(vector_dim, 0.2f);
    std::vector<float> vec3(vector_dim, 0.3f);

    db.insert_vector(1, vec1);
    db.insert_vector(2, vec2);
    db.insert_vector(3, vec3);

    // Query vectors
    auto results = db.query_vector(vec1, 2);
    assert(results.size() == 2);
    assert(results[0].first == 1); // Nearest to itself

    // Depending on KD-tree implementation, second nearest might vary
    std::cout << "TestBuzzDB Passed!" << std::endl;
    return 0;
}
