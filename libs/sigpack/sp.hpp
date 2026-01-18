#pragma once

#include "object.hpp"

class SigpackInterface : public NativeClass {
public:
    SigpackInterface();
    ~SigpackInterface();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value call(ObjectFactory* factory, int argc, Value* argv);
    Value constant(ObjectFactory* factory, std::string name);
};
