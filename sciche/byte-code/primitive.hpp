#pragma once

#include "object.hpp"

#include <string>

typedef Value (*NativeBooundFn)(ObjectFactory* factory, Obj* obj, int argc, Value* argv);

class Primitive {
public:
    NativeBooundFn find(ObjType type, std::string name);
    Value call(ObjectFactory* factory, Value value, std::string name, int argc, Value* argv);
};
