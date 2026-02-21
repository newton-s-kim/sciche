#pragma once

#include "object.hpp"

#include <string>

namespace sce {
class Primitive {
public:
    Value call(ObjectFactory* factory, Value value, std::string& name, int argc, Value* argv);
    Value property(ObjectFactory* factory, Value value, std::string& name);
};
} // namespace sce
