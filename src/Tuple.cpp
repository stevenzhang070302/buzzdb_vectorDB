// src/Tuple.cpp
#include "Tuple.hpp"

// Add Field
void Tuple::addField(std::unique_ptr<Field> field) {
    fields.push_back(std::move(field));
}

// Get Size
size_t Tuple::getSize() const {
    size_t size = 0;
    for (const auto& field : fields) {
        size += field->data_length;
    }
    return size;
}

// Serialization
std::string Tuple::serialize() const {
    std::stringstream buffer;
    buffer << fields.size() << ' ';
    for (const auto& field : fields) {
        buffer << field->serialize();
    }
    return buffer.str();
}

void Tuple::serialize(std::ofstream& out) const {
    std::string serializedData = this->serialize();
    out << serializedData;
}

// Deserialization
std::unique_ptr<Tuple> Tuple::deserialize(std::istream& in) {
    auto tuple = std::make_unique<Tuple>();
    size_t fieldCount; in >> fieldCount;
    for (size_t i = 0; i < fieldCount; ++i) {
        tuple->addField(Field::deserialize(in));
    }
    return tuple;
}

// Print
void Tuple::print() const {
    for (const auto& field : fields) {
        field->print();
        std::cout << " ";
    }
    std::cout << "\n";
}
