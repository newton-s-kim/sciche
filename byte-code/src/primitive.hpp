#pragma once

#include "object.hpp"

#include <string>

class Primitive {
public:
    Value call(ObjectFactory* factory, Value value, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, Value value, std::string name);
};
