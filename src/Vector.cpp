// src/Vector.cpp
#include "Vector.hpp"

// Constructors
Vector::Vector() : data(Eigen::VectorXf::Zero(0)) {}

Vector::Vector(const std::vector<float>& vec) : data(Eigen::VectorXf::Map(vec.data(), vec.size())) {}

// Dot Product
float Vector::dot(const Vector& other) const {
    return data.dot(other.data);
}

// Magnitude
float Vector::magnitude() const {
    return data.norm();
}

// Cosine Similarity
float Vector::cosine_similarity(const Vector& other) const {
    return this->dot(other) / (this->magnitude() * other.magnitude());
}

// Euclidean Distance
float Vector::euclidean_distance(const Vector& other) const {
    return (this->data - other.data).norm();
}
