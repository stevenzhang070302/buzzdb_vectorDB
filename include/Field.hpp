// include/Field.hpp
#ifndef FIELD_HPP
#define FIELD_HPP

#include <string>
#include <memory>
#include <cstring>
#include <sstream>
#include <iostream>

enum FieldType { INT, FLOAT, STRING };

// Define a basic Field variant class that can hold different types
class Field {
public:
    FieldType type;
    std::unique_ptr<char[]> data;
    size_t data_length;

public:
    Field(int i);
    Field(float f);
    Field(const std::string& s);
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept;
    FieldType getType() const;
    int asInt() const;
    float asFloat() const;
    std::string asString() const;
    std::string serialize() const;
    void serialize(std::ofstream& out) const;
    static std::unique_ptr<Field> deserialize(std::istream& in);
    void print() const;
};

#endif // FIELD_HPP
