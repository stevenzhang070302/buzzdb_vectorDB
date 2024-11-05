// include/Vector.hpp
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include <Eigen/Dense>
#include <iostream>

class Vector {
public:
    Eigen::VectorXf data;

    Vector();
    Vector(const std::vector<float>& vec);
    float dot(const Vector& other) const;
    float magnitude() const;
    float cosine_similarity(const Vector& other) const;
    float euclidean_distance(const Vector& other) const;
};

#endif // VECTOR_HPP
