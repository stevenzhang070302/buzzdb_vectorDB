// src/VectorPage.cpp
#include "VectorPage.hpp"

// Constructor
VectorPage::VectorPage() : data(std::make_unique<char[]>(PAGE_SIZE)) {
    std::memset(data.get(), 0, PAGE_SIZE);
}

// Insert Vector
bool VectorPage::insert_vector(const std::vector<float>& vector, size_t offset) {
    if (offset + vector.size() * sizeof(float) > PAGE_SIZE) {
        return false; // Not enough space
    }
    std::memcpy(data.get() + offset, vector.data(), vector.size() * sizeof(float));
    return true;
}

// Get Vector
std::vector<float> VectorPage::get_vector(size_t offset, size_t dimension) const {
    std::vector<float> vector(dimension);
    std::memcpy(vector.data(), data.get() + offset, dimension * sizeof(float));
    return vector;
}
