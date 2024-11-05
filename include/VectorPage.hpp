// include/VectorPage.hpp
#ifndef VECTORPAGE_HPP
#define VECTORPAGE_HPP

#include <vector>
#include <memory>
#include <cstring>
#include <iostream>

class VectorPage {
public:
    std::unique_ptr<char[]> data;

    VectorPage();
    bool insert_vector(const std::vector<float>& vector, size_t offset);
    std::vector<float> get_vector(size_t offset, size_t dimension) const;
};

#endif // VECTORPAGE_HPP
