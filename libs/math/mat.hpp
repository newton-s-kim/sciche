#pragma once
#include "object.hpp"

namespace sce {
class vecNative : public NativeClass {
public:
    vecNative();
    Value call(ObjectFactory* factory, int argc, Value* argv);
};

class matNative : public NativeClass {
public:
    matNative();
    Value call(ObjectFactory* factory, int argc, Value* argv);
};
} // namespace sce
