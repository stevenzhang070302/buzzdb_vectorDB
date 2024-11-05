// src/Field.cpp
#include "Field.hpp"

// Constructors
Field::Field(int i) : type(INT) { 
    data_length = sizeof(int);
    data = std::make_unique<char[]>(data_length);
    std::memcpy(data.get(), &i, data_length);
}

Field::Field(float f) : type(FLOAT) { 
    data_length = sizeof(float);
    data = std::make_unique<char[]>(data_length);
    std::memcpy(data.get(), &f, data_length);
}

Field::Field(const std::string& s) : type(STRING) {
    data_length = s.size() + 1;  // include null-terminator
    data = std::make_unique<char[]>(data_length);
    std::memcpy(data.get(), s.c_str(), data_length);
}

// Assignment Operator
Field& Field::operator=(const Field& other) {
    if (&other == this) {
        return *this;
    }
    type = other.type;
    data_length = other.data_length;
    data = std::make_unique<char[]>(data_length);
    std::memcpy(data.get(), other.data.get(), data_length);
    return *this;
}

// Move Constructor
Field::Field(Field&& other) noexcept {
    type = other.type;
    data_length = other.data_length;
    data = std::move(other.data);
}

// Getters
FieldType Field::getType() const { return type; }

int Field::asInt() const { 
    return *reinterpret_cast<int*>(data.get());
}

float Field::asFloat() const { 
    return *reinterpret_cast<float*>(data.get());
}

std::string Field::asString() const { 
    return std::string(data.get());
}

// Serialization
std::string Field::serialize() const {
    std::stringstream buffer;
    buffer << type << ' ' << data_length << ' ';
    if (type == STRING) {
        buffer << data.get() << ' ';
    } else if (type == INT) {
        buffer << *reinterpret_cast<int*>(data.get()) << ' ';
    } else if (type == FLOAT) {
        buffer << *reinterpret_cast<float*>(data.get()) << ' ';
    }
    return buffer.str();
}

void Field::serialize(std::ofstream& out) const {
    std::string serializedData = this->serialize();
    out << serializedData;
}

std::unique_ptr<Field> Field::deserialize(std::istream& in) {
    int type_int; in >> type_int;
    size_t length; in >> length;
    if (type_int == STRING) {
        std::string val; in >> val;
        return std::make_unique<Field>(val);
    } else if (type_int == INT) {
        int val; in >> val;
        return std::make_unique<Field>(val);
    } else if (type_int == FLOAT) {
        float val; in >> val;
        return std::make_unique<Field>(val);
    }
    return nullptr;
}

// Print
void Field::print() const{
    switch(getType()){
        case INT: std::cout << asInt(); break;
        case FLOAT: std::cout << asFloat(); break;
        case STRING: std::cout << asString(); break;
    }
}
