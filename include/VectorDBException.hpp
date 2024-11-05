// include/VectorDBException.hpp
#ifndef VECTORDBEXCEPTION_HPP
#define VECTORDBEXCEPTION_HPP

#include <exception>
#include <string>

class VectorDBException : public std::exception {
private:
    std::string message;
    int status_code;

public:
    VectorDBException(const std::string& msg, int code = 400) : message(msg), status_code(code) {}
    const char* what() const noexcept override { return message.c_str(); }
    int getStatusCode() const { return status_code; }
};

#endif // VECTORDBEXCEPTION_HPP
