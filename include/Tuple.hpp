// include/Tuple.hpp
#ifndef TUPLE_HPP
#define TUPLE_HPP

#include "Field.hpp"
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>

class Tuple {
public:
    std::vector<std::unique_ptr<Field>> fields;

    void addField(std::unique_ptr<Field> field);
    size_t getSize() const;
    std::string serialize() const;
    void serialize(std::ofstream& out) const;
    static std::unique_ptr<Tuple> deserialize(std::istream& in);
    void print() const;
};

#endif // TUPLE_HPP
