#pragma once

#include <stdexcept>

class ServExcept : public std::runtime_error
{
public:
    explicit ServExcept(const std::string &msg)
        : std::runtime_error("Server Error: " + msg) {}

};


