#pragma once

#include "object.hpp"

namespace sce {
class SigpackInterface : public NativeClass {
public:
    SigpackInterface(ObjectFactory* factory);
    ~SigpackInterface();
};
} // namespace sce
